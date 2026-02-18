#pragma once
#include "GameplayTagContainer.h"
#include "ResourceRule.h"

#include "BiomeDef.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FBiomeDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Identity")
	FGameplayTag BiomeTag;

	UPROPERTY(EditDefaultsOnly, Category="Generation")
	FFloatRange Temperature;
	UPROPERTY(EditDefaultsOnly, Category="Generation")
	FFloatRange Humidity;
	
	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TArray<FResourceRule> Resources;
	
	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	FLinearColor Color = FLinearColor::White;
};