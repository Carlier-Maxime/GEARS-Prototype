#pragma once
#include "Data/Core/ClimatePoint.h"
#include "GameplayTagContainer.h"
#include "Data/Core/ResourceRule.h"

#include "BiomeDef.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FBiomeDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Identity")
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly, Category="Generation")
	TArray<FClimatePoint> ClimateInfluences = {FClimatePoint()};
	
	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TArray<FResourceRule> Resources;
	
	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	FLinearColor Color = FLinearColor::White;
};