// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StyleData.h"
#include "Slate/SlateBrushAsset.h"
#include "SliderStyleData.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API USliderStyleData : public UStyleData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Style")
	TObjectPtr<USlateBrushAsset> BarBrush;
	UPROPERTY(EditDefaultsOnly, Category="Style")
	TObjectPtr<USlateBrushAsset> ThumbBrush;
	UPROPERTY(EditDefaultsOnly, Category="Style")
	float BarThickness = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category="States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HoverLightIntensity = 0.15f;
	UPROPERTY(EditDefaultsOnly, Category="States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PressedDarkIntensity = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category="States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DisabledAlphaIntensity = 0.5f;
};
