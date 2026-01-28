// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StyleData.h"
#include "Slate/SlateBrushAsset.h"
#include "ButtonStyleData.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UButtonStyleData : public UStyleData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Style")
	TObjectPtr<USlateBrushAsset> MainBrush = nullptr;
	UPROPERTY(EditDefaultsOnly, Category="Style")
	FLinearColor ForegroundColor = FLinearColor::White;
	UPROPERTY(EditDefaultsOnly, Category = "Style")
	FMargin Padding = FMargin(4.f);
	
	UPROPERTY(EditDefaultsOnly, Category = "States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HoverLightIntensity = 0.15f;
	UPROPERTY(EditDefaultsOnly, Category = "States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PressedDarkIntensity = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category = "States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DisabledAlphaIntensity = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> PressedSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> ClickedSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> HoveredSound;
};
