// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MoveTo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NavigationInvokerComponent.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameplayTags/GEARS_GameplayTags.h"
#include "Navigation/PathFollowingComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UGA_MoveTo::UGA_MoveTo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_Ability_Move_Pathfinding;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
	
	ActivationOwnedTags.AddTag(TAG_State_Moving_Auto);
	SetAssetTags(FGameplayTagContainer{TAG_Ability_Move_Pathfinding});
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Move_Pathfinding);
	
}

void UGA_MoveTo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FHitResult Hit;
	if (!TryHitResultFrom(TriggerEventData, Hit)) return EndCancel();
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return EndCancel();
	CurrentEventData = *TriggerEventData;
	if (MoveTask) 
	{
		MoveTask->EndTask();
		MoveTask = nullptr;
	}
	bMoveHasFinished = false;

	Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	DesiredLocation = Hit.Location;
	ExtentValBase = TriggerEventData->EventMagnitude>0 ? TriggerEventData->EventMagnitude : 1;
	if (!MoveToLocation()) return EndCancel();
	FGameplayCueParameters Params;
	Params.Location = DesiredLocation;
	Params.Normal = Hit.ImpactNormal;
	K2_ExecuteGameplayCueWithParams(TAG_Cue_Move_Confirm, Params);
}

bool UGA_MoveTo::MoveToLocation()
{
	if (!Pawn) return false;
	auto* Controller = Cast<AController>(Pawn->GetController());
	if (!Controller) return false;
	auto* PFC = Controller->GetComponentByClass<UPathFollowingComponent>();
	auto* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const auto* NavInvoker = Pawn->FindComponentByClass<UNavigationInvokerComponent>();
	if (!NavSys || !NavInvoker || !PFC) return false;
	
	if (!TickDelegateHandle.IsValid()) TickDelegateHandle = PFC->OnRequestFinished.AddUObject(this, &UGA_MoveTo::OnMoveFinished);
	
	const auto Radius = NavInvoker->GetGenerationRadius();
	const auto StartLoc = Pawn->GetActorLocation();
	const auto DistToLoc = FVector::Dist(DesiredLocation, StartLoc);
	double ExtentVal = ExtentValBase;
	if (DistToLoc > Radius) ExtentVal += DistToLoc - Radius;
	const FVector Extent(ExtentVal, ExtentVal, ExtentVal);
	FNavLocation NavLoc;
	if (!NavSys->ProjectPointToNavigation(DesiredLocation, NavLoc, Extent)) return false;
	
	FAIMoveRequest MoveReq(NavLoc.Location);
	MoveReq.SetUsePathfinding(true);
	const auto* NavData = NavSys->GetNavDataForProps(Pawn->GetNavAgentPropertiesRef());
	if (!NavData) return false;
	FPathFindingQuery Query(Pawn, *NavData, Pawn->GetActorLocation(), NavLoc.Location);
	auto PathResult = NavSys->FindPathSync(Query);
	if (!PathResult.IsSuccessful()) return false;
	PFC->RequestMove(MoveReq, PathResult.Path);
	
	if (DistToLoc > Radius) LoopPathCheck(NavLoc.Location);
	return true;
}

void UGA_MoveTo::LoopPathCheck(const FVector& IntermediateLocation)
{
	const auto* MoveComponent = Pawn->GetMovementComponent();
	if (!MoveComponent) return;
	const auto ElapsedHalfTime = 0.5*FVector::Dist(IntermediateLocation, Pawn->GetActorLocation()) / MoveComponent->GetMaxSpeed();
	if (ElapsedHalfTime < 0.1) return;
	
	MoveTask = UAbilityTask_WaitDelay::WaitDelay(this, ElapsedHalfTime);
	MoveTask->OnFinish.AddUniqueDynamic(this, &UGA_MoveTo::OnTickPathUpdate);
	MoveTask->ReadyForActivation();
}

void UGA_MoveTo::OnTickPathUpdate()
{
	MoveToLocation();
}

void UGA_MoveTo::OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsInterrupted()) return;
	bMoveHasFinished = true;
	EndAbilitySimple(true, !Result.IsSuccess());
}

void UGA_MoveTo::UnboundTickDelegate()
{
	if (Pawn && Pawn->GetController())
	{
		auto* PFC = Pawn->GetController()->GetComponentByClass<UPathFollowingComponent>();
		if (PFC) PFC->OnRequestFinished.Remove(TickDelegateHandle);
	}
	TickDelegateHandle.Reset();
}

void UGA_MoveTo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const auto Payload = std::move(CurrentEventData);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnboundTickDelegate();
	auto* Actor = GetAvatarActorFromActorInfo();
	auto EventTag = bWasCancelled || !bMoveHasFinished ? &TAG_Event_Movement_Cancel : &TAG_Event_Movement_Arrived;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, *EventTag, Payload);
}
