// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Common.h"
#include "GA_Interact.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitDelay;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGA_Interact : public UGA_Common
{
	GENERATED_BODY()
public:
	UGA_Interact();

protected:
	UFUNCTION()
	void OnMoveArrived(FGameplayEventData Payload);
	UFUNCTION()
	void OnMoveCanceled(FGameplayEventData Payload);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	void InitMoveTask();
	void ClearMoveTask();
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* MoveArrivedTask;
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* MoveCanceledTask;
};
