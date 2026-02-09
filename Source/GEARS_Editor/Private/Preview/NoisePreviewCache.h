#pragma once

class BaseGenerator;
struct FGridPosition;

struct FNoisePreviewCache
{
	FNoisePreviewCache();
	
	TStrongObjectPtr<UTexture2D> Texture = nullptr;
	FSlateBrush Brush;
	int32 Resolution = 256;
	int32 Seed = 0;
	void* StructPtr = nullptr;
	FName StructName;
	
	void Update();
private:
	FColor GenerateColorAtPos(const BaseGenerator& Generator, const FGridPosition& Pos) const;
};
