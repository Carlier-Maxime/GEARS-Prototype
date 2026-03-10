// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Common.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Grid/Rendering/WorldRenderer.h"

void UGA_Common::EndCancel(bool bReplicateEndAbility)
{
	EndAbilitySimple(bReplicateEndAbility, true);
}

void UGA_Common::EndFinish(bool bReplicateEndAbility)
{
	EndAbilitySimple(bReplicateEndAbility, false);
}

void UGA_Common::EndAbilitySimple(bool bReplicateEndAbility, bool bWasCancelled)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_Common::TryHitResultFrom(const FGameplayEventData* EventData, FHitResult& OutHit, AWorldRenderer** OutRenderer, UAbilitySystemComponent** OutASC) const
{
	if (!EventData || !EventData->TargetData.IsValid(0) || !CurrentActorInfo || !CurrentActorInfo->OwnerActor.IsValid()) return false;
	OutHit = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(EventData->TargetData, 0);
	if (OutASC)
	{
		*OutASC = GetAbilitySystemComponentFromActorInfo();
		if (!*OutASC) return false;
	}
	if (OutRenderer)
	{
		*OutRenderer = Cast<AWorldRenderer>(OutHit.GetActor());
		if (!*OutRenderer) return false;
	}
	return true;
}
