#pragma once

#include "PlacementRule.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FPlacementRule
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Transform", meta = (ClampMin = 0, ClampMax = 0.3))
	float JitterMaxOffset = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Transform")
	TObjectPtr<UCurveFloat> ScaleCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bRandomYaw = true;
};
