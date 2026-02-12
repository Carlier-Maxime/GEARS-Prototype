#pragma once

#include "CoreMinimal.h"
#include "NoisePreviewSettings.h"

struct FGridPosition;

DECLARE_DELEGATE_RetVal_OneParam(FColor, FOnGenerateColor, FGridPosition)

struct FNoisePreviewState
{
	FNoisePreviewState();
	
	TStrongObjectPtr<UTexture2D> Texture = nullptr;
	FSlateBrush Brush;
	FNoisePreviewSettings Settings;
	FOnGenerateColor OnGenerateColor;
	
	void Update();
};
