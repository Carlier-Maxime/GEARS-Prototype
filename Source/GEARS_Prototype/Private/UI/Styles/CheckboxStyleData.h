// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StyleData.h"
#include "Slate/SlateBrushAsset.h"
#include "CheckboxStyleData.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UCheckboxStyleData : public UStyleData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Style")
	TObjectPtr<USlateBrushAsset> MainBrush;
	UPROPERTY(EditDefaultsOnly, Category="Style", meta=(DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TObjectPtr<UObject> CheckedImage;
	UPROPERTY(EditDefaultsOnly, Category="Style", meta=(DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TObjectPtr<UObject> UndeterminedImage;
	UPROPERTY(EditDefaultsOnly, Category="Style")
	FMargin Padding = FMargin(4.f);
	UPROPERTY(EditDefaultsOnly, Category="Style")
	bool bCheckedTintWhite = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HoverLightIntensity = 0.15f;
	UPROPERTY(EditDefaultsOnly, Category = "States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PressedDarkIntensity = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category = "States", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float UndeterminedAlphaIntensity = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<USoundBase> CheckedSound;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<USoundBase> UncheckedSound;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<USoundBase> HoveredSound;
};
