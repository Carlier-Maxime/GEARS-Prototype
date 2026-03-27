// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Harvest.h"

#include "GameplayTags/GEARS_GameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animations/Interfaces/AnimIKInterface.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Grid/GridSubsystem.h"

UGA_Harvest::UGA_Harvest()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_Event_Interact_Execute;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
	
	ActivationBlockedTags.AddTag(TAG_State_Moving);
	TargetRequiredTags.AddTag(TAG_Resource);
	SetAssetTags(FGameplayTagContainer{TAG_Ability_Interact_Harvest});
	ActivationOwnedTags.AddTag(TAG_State_Harvesting);
}

void UGA_Harvest::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FHitResult Hit;
	AWorldRenderer* Renderer = nullptr;
	UAbilitySystemComponent* ASC = nullptr;
	if (!TryHitResultFrom(TriggerEventData, Hit, &Renderer, &ASC)) return EndCancel();
	auto HISM = Cast<UHierarchicalInstancedStaticMeshComponent>(Hit.Component);
	if (!HISM || Hit.Item == INDEX_NONE) return EndCancel();
	
	auto* WaitTag = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_State_Moving);
	WaitTag->Added.AddUniqueDynamic(this, &UGA_Harvest::OnMoveStarted);
	WaitTag->ReadyForActivation();
	
	RefreshMiningHit(Hit, TriggerEventData->EventMagnitude, HISM);
	auto* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_Harvest_Impact, nullptr, false);
	WaitEvent->EventReceived.AddUniqueDynamic(this, &ThisClass::OnImpact);
	WaitEvent->ReadyForActivation();
	
	if (AnimMontage) ImpactFromMontage();
	else OneImpactAndStop();
}

void UGA_Harvest::OnMoveStarted()
{
	EndFinish();
}

void UGA_Harvest::RefreshMiningHit(const FHitResult& BaseHit, const double Marge, UHierarchicalInstancedStaticMeshComponent* HISM)
{
	FTransform InstanceTr;
	if (!HISM) HISM = Cast<UHierarchicalInstancedStaticMeshComponent>(BaseHit.Component);
	HISM->GetInstanceTransform(BaseHit.Item, InstanceTr, true);
	MiningResourcePos = FWorldGridPos(InstanceTr.GetLocation());
	auto TargetBounds = HISM->GetStaticMesh()->GetBoundingBox();
	auto MaxZ = InstanceTr.TransformPosition(TargetBounds.GetCenter()).Z + InstanceTr.GetScale3D().Z * TargetBounds.GetExtent().Z;
	auto StartTrace = CurrentActorInfo->OwnerActor->GetActorLocation();
	auto Vec = FVector(BaseHit.Location.X, BaseHit.Location.Y, FMath::Min(MaxZ, StartTrace.Z)) - StartTrace;
	auto EndTrace = StartTrace + Vec.GetSafeNormal() * FMath::Max(Marge>1e-5 ? Marge : 1, Vec.Size());
	
	FHitResult NewMiningHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CurrentActorInfo->OwnerActor.Get());
	Params.bTraceComplex = true;
	if (!GetWorld()->LineTraceSingleByChannel(NewMiningHit, StartTrace, EndTrace, ECC_Visibility, Params)) MiningHit = BaseHit;
	else if (NewMiningHit.Item != BaseHit.Item || NewMiningHit.Component != BaseHit.Component) MiningHit = BaseHit;
	else MiningHit = NewMiningHit;
}

void UGA_Harvest::OnImpact(FGameplayEventData Payload)
{
	auto* Grid = GetWorld()->GetSubsystem<UGridSubsystem>();
	if (!Grid) return;
	const auto DamageResult = Grid->ApplyDamageToResource(MiningResourcePos, 1, CurrentActorInfo->OwnerActor.Get());
	if (DamageResult == EDamageResult::None) return;
	DrawDebugPoint(GetWorld(), MiningHit.Location, 10, FColor::Red, false, 0.25); // TODO send event for Gameplay Cue (feedback)
	if (DamageResult == EDamageResult::Destroyed) EndFinish();
}

void UGA_Harvest::ImpactFromMontage()
{
	auto* Proxy = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AnimMontage);
	Proxy->OnCompleted.AddDynamic(this, &ThisClass::EndFinish);
	Proxy->OnInterrupted.AddDynamic(this, &ThisClass::EndFinish);
	Proxy->OnCancelled.AddDynamic(this, &ThisClass::EndFinish);
	
	if (CurrentActorInfo->OwnerActor.IsValid())
	{
		if (const auto* Mesh = CurrentActorInfo->OwnerActor.Get()->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (auto* IKInterface = Cast<IAnimIKInterface>(Mesh->GetAnimInstance()))
			{
				IKInterface->GetIK(TAG_IK_Arm_Left).Target.SetLocation(MiningHit.Location);
				IKInterface->GetIK(TAG_IK_Arm_Right).Target.SetLocation(MiningHit.Location);
			}
		}
	}
	
	Proxy->ReadyForActivation();
}

void UGA_Harvest::OneImpactAndStop()
{
	FGameplayEventData Payload;
	Payload.EventTag = TAG_Event_Harvest_Impact;
	Payload.Instigator = CurrentActorInfo->OwnerActor.Get();
	OnImpact(Payload);
	EndFinish();
}
