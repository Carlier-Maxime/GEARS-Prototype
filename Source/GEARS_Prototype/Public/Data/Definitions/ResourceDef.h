#pragma once
#include "GameplayTagContainer.h"
#include "Data/Core/PlacementRule.h"
#include "Data/States/ResourceState.h"

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
	
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FResourceState State;
};