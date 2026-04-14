#pragma once

#include "ResourceState.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FResourceState
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = 0.01, UIMin=1, UIMax=1000))
	float Health = 10.0f;
	
	UPROPERTY()
	float AccumulatedLootDamage = 0;
};