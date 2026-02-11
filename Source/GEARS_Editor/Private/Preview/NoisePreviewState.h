#pragma once

#include "CoreMinimal.h"
#include "NoisePreviewSettings.h"
#include "Grid/Generator/BaseGenerator.h"
#include "Grid/Generator/ResourceGenerator.h"

struct FGridPosition;

struct FNoisePreviewGenerators
{
	BaseGenerator Base;
	ResourceGenerator Resource;
};

struct FNoisePreviewState
{
	FNoisePreviewState();
	
	TStrongObjectPtr<UTexture2D> Texture = nullptr;
	FSlateBrush Brush;
	FNoisePreviewSettings Settings;
	void* StructPtr = nullptr;
	FName StructName;
	
	void Update();
private:
	FColor GenerateColorAtPos(const FNoisePreviewGenerators& Generator, const FGridPosition& Pos) const;
};
