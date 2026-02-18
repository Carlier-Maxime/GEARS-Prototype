#pragma once
#include "GameplayTagContainer.h"
#include "Data/Definitions/PlacementRule.h"
#include "Grid/Generator/Context/DistributionRule.h"

#include "ResourceDef.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FResourceDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	FGameplayTag Tag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling")
	FPlacementRule PlacementRule;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling")
	FDistributionRule Distribution;
};