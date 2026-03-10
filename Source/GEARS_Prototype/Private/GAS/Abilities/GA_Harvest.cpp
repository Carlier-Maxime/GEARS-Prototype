// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Harvest.h"

#include "GameplayTags/GEARS_GameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

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
	
	auto* WaitTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_State_Moving);
	WaitTask->Added.AddUniqueDynamic(this, &UGA_Harvest::OnMoveStarted);
	WaitTask->ReadyForActivation();
	
	const auto MiningHit = CalcMiningHit(Hit, TriggerEventData->EventMagnitude, HISM);
	DrawDebugPoint(GetWorld(), MiningHit.Location, 10, FColor::Red, false, 2); // TODO Implement Harvesting
}

void UGA_Harvest::OnMoveStarted()
{
	EndFinish();
}

FHitResult UGA_Harvest::CalcMiningHit(const FHitResult& BaseHit, const double Marge, UHierarchicalInstancedStaticMeshComponent* HISM) const
{
	FTransform InstanceTr;
	if (!HISM) HISM = Cast<UHierarchicalInstancedStaticMeshComponent>(BaseHit.Component);
	HISM->GetInstanceTransform(BaseHit.Item, InstanceTr, true);
	auto TargetBounds = HISM->GetStaticMesh()->GetBoundingBox();
	auto MaxZ = InstanceTr.TransformPosition(TargetBounds.GetCenter()).Z + InstanceTr.GetScale3D().Z * TargetBounds.GetExtent().Z;
	auto StartTrace = CurrentActorInfo->OwnerActor->GetActorLocation();
	auto Vec = FVector(BaseHit.Location.X, BaseHit.Location.Y, FMath::Min(MaxZ, StartTrace.Z)) - StartTrace;
	auto EndTrace = StartTrace + Vec.GetSafeNormal() * FMath::Max(Marge>1e-5 ? Marge : 1, Vec.Size());
	
	FHitResult MiningHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CurrentActorInfo->OwnerActor.Get());
	Params.bTraceComplex = true;
	if (!GetWorld()->LineTraceSingleByChannel(MiningHit, StartTrace, EndTrace, ECC_Visibility, Params)) return BaseHit;
	if (MiningHit.Item != BaseHit.Item || MiningHit.Component != BaseHit.Component) return BaseHit;
	return MiningHit;
}
