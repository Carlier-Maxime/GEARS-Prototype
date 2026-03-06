// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Interact.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags/GEARS_GameplayTags.h"
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
	if (!TriggerEventData || !TriggerEventData->TargetData.IsValid(0) || !ActorInfo->OwnerActor.IsValid()) return EndCancel();
	const auto Hit = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);
	const auto* Renderer = Cast<AWorldRenderer>(Hit.GetActor());
	if (!Renderer || !Hit.Component.IsValid()) return;
	auto* Component = Hit.Component.Get();
	
	FGameplayEventData Payload;
	Payload.TargetData = TriggerEventData->TargetData;
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
	
	if (Renderer->IsPlane(Component)) AbilityTag = TAG_Ability_Move_Pathfinding;
	else if (Renderer->IsResourceComponent(Component)) UE_LOG(LogTemp, Warning, TEXT("TODO Implement Mine Resource")); // TODO
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActorInfo->OwnerActor.Get(), AbilityTag, Payload);
}
