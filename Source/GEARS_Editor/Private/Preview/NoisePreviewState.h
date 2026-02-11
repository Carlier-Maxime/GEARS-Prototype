#pragma once
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
	int32 Resolution = 256;
	int32 Seed = 0;
	void* StructPtr = nullptr;
	FName StructName;
	
	void Update();
private:
	FColor GenerateColorAtPos(const FNoisePreviewGenerators& Generator, const FGridPosition& Pos) const;
};
