// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlacementRule.h"
#include "Engine/DataAsset.h"
#include "Grid/Generator/Context/DistributionRule.h"
#include "ResourceType.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UResourceType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FGameplayTag ResourceTag;
	
	UPROPERTY(EditAnywhere, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditAnywhere, Category = "Sampling")
	FPlacementRule PlacementRule;
	
	UPROPERTY(EditAnywhere, Category = "Sampling")
	FDistributionRule Distribution;
};
