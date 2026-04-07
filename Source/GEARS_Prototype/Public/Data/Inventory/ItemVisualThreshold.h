#pragma once
#include "ItemVisual.h"

#include "ItemVisualThreshold.generated.h"

USTRUCT()
struct FItemVisualThreshold
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1))
	int32 MinQuantity = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FItemVisual Visual;
};