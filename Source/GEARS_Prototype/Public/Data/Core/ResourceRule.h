#pragma once

#include "GameplayTagContainer.h"
#include "Grid/Generator/Context/DistributionRule.h"
#include "ResourceRule.generated.h"

class UResourceType;

USTRUCT()
struct GEARS_PROTOTYPE_API FResourceRule
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (Categories = "Resource"))
	FGameplayTag ResourceTag;
	
	UPROPERTY(EditAnywhere)
	FDistributionRule Distribution;
};
