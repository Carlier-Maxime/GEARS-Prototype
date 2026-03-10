// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "GA_Common.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GA_MoveTo.generated.h"

struct FPathFollowingResult;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGA_MoveTo : public UGA_Common
{
	GENERATED_BODY()
public:
	UGA_MoveTo();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	bool MoveToLocation();
	UFUNCTION()
	void OnTickPathUpdate();
	void LoopPathCheck(const FVector& IntermediateLocation);
	void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void UnboundTickDelegate();
	
	FDelegateHandle TickDelegateHandle;
	UPROPERTY()
	APawn* Pawn;
	FVector DesiredLocation;
	float ExtentValBase;
	UPROPERTY()
	UAbilityTask_WaitDelay* MoveTask;
	bool bMoveHasFinished = false;
};
