// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Common.h"
#include "Grid/Types/WorldGridPos.h"
#include "GA_Harvest.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGA_Harvest : public UGA_Common
{
	GENERATED_BODY()
public:
	UGA_Harvest();

protected:
	UFUNCTION()
	void OnImpact(FGameplayEventData Payload);
	void OneImpactAndStop();
	void ImpactFromMontage();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
private:
	UFUNCTION()
	void OnMoveStarted();
	void RefreshMiningHit(const FHitResult& BaseHit, double Marge, UHierarchicalInstancedStaticMeshComponent* HISM = nullptr);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;
	
	FHitResult MiningHit;
	FTransform MiningResourceTr;
};
