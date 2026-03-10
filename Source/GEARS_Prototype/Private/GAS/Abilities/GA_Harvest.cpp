// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Harvest.h"

#include "GameplayTags/GEARS_GameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

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
	auto* WaitTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_State_Moving);
	WaitTask->Added.AddUniqueDynamic(this, &UGA_Harvest::OnMoveStarted);
	WaitTask->ReadyForActivation();
	
	UE_LOG(LogTemp, Warning, TEXT("Harvest Ability Activated")); // TODO Implement Harvesting
}

void UGA_Harvest::OnMoveStarted()
{
	EndFinish();
}
