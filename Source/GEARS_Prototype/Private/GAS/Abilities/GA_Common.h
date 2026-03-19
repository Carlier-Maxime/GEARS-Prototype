// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Common.generated.h"

class AWorldRenderer;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGA_Common : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	void EndCancel(bool bReplicateEndAbility);
	UFUNCTION()
	FORCEINLINE void EndCancel();
	void EndFinish(bool bReplicateEndAbility);
	UFUNCTION()
	FORCEINLINE void EndFinish();
	void EndAbilitySimple(bool bReplicateEndAbility, bool bWasCancelled);
	bool TryHitResultFrom(const FGameplayEventData* EventData, FHitResult& OutHit, AWorldRenderer** OutRenderer = nullptr, UAbilitySystemComponent** OutASC = nullptr) const;
};

inline void UGA_Common::EndCancel()
{
	EndCancel(true);
}

inline void UGA_Common::EndFinish()
{
	EndFinish(true);
}
