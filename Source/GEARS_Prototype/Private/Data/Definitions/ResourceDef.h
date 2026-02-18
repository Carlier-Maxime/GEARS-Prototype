#pragma once
#include "GameplayTagContainer.h"
#include "Data/PlacementRule.h"
#include "Grid/Generator/Context/DistributionRule.h"

#include "ResourceDef.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FResourceDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	FGameplayTag ResourceTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling")
	FPlacementRule PlacementRule;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling")
	FDistributionRule Distribution;
};