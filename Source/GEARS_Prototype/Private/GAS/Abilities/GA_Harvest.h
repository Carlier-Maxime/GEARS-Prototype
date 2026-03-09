// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Harvest.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGA_Harvest : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Harvest();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
private:
	UFUNCTION()
	void OnMoveStarted();
};
