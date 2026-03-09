// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Interact.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayTags/GEARS_GameplayTags.h"
#include "GAS/Attributes/CharacterAttributeSet.h"
#include "Grid/Rendering/WorldRenderer.h"

UGA_Interact::UGA_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_Ability_Interact;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
	
	SetAssetTags(FGameplayTagContainer{TAG_Ability_Interact});
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Interact);
}

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	auto EndCancel = [&]{EndAbility(Handle, ActorInfo, ActivationInfo, true, true);};
	
	ClearMoveTask();
	if (!TriggerEventData || !TriggerEventData->TargetData.IsValid(0) || !ActorInfo->OwnerActor.IsValid()) return EndCancel();
	const auto Hit = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);
	const auto* Renderer = Cast<AWorldRenderer>(Hit.GetActor());
	auto* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !Renderer || !Hit.Component.IsValid()) return EndCancel();
	auto* Component = Hit.Component.Get();
	
	FGameplayEventData Payload;
	Payload.TargetData = TriggerEventData->TargetData;
	
	if (Renderer->IsPlane(Component))
	{
		Payload.EventMagnitude = ASC->GetNumericAttribute(UCharacterAttributeSet::GetArrivalPrecisionAttribute());;
	} else if (Renderer->IsResourceComponent(Component))
	{
		Payload.EventMagnitude = ASC->GetNumericAttribute(UCharacterAttributeSet::GetHarvestRangeAttribute());
	}
	
	InitMoveTask();
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActorInfo->OwnerActor.Get(), TAG_Ability_Move_Pathfinding, Payload);
}

void UGA_Interact::InitMoveTask()
{
	MoveArrivedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		TAG_Event_Movement_Arrived, 
		nullptr,
		true
	);
	MoveArrivedTask->EventReceived.AddUniqueDynamic(this, &UGA_Interact::OnMoveArrived);
	MoveArrivedTask->ReadyForActivation();
	
	MoveCanceledTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		TAG_Event_Movement_Cancel, 
		nullptr,
		true
	);
	MoveCanceledTask->EventReceived.AddUniqueDynamic(this, &UGA_Interact::OnMoveCanceled);
	MoveCanceledTask->ReadyForActivation();
}

void UGA_Interact::ClearMoveTask()
{
	if (MoveArrivedTask)
	{
		MoveArrivedTask->EndTask();
		MoveArrivedTask = nullptr;
	}
	if (MoveCanceledTask)
	{
		MoveCanceledTask->EndTask();
		MoveCanceledTask = nullptr;
	}
}

void UGA_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearMoveTask();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Interact::OnMoveArrived(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Arrived to destination for interact !")); //TODO 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Interact::OnMoveCanceled(FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
