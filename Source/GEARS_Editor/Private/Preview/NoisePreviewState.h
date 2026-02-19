#pragma once

#include "CoreMinimal.h"
#include "NoisePreviewContext.h"

struct FWorldGridPos;

DECLARE_DELEGATE_RetVal_OneParam(FColor, FOnGenerateColor, FWorldGridPos)

struct FNoisePreviewState
{
	FNoisePreviewState();
	
	TArray<FColor> PixelBuffer;
	TStrongObjectPtr<UTexture2D> Texture = nullptr;
	FSlateBrush Brush;
	FOnGenerateColor OnGenerateColor;
	
	void Update(const FNoisePreviewContext& Ctx);
};
