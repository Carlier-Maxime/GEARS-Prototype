#pragma once
#include "GridSubsystem.h"

class WorldGenerator
{
public:
	WorldGenerator(UGridSubsystem& Grid, int32 Seed);
	[[nodiscard]] TArray<TArray<FTransform>> Generate(const uint16 ChunkRadius);
	[[nodiscard]] TArray<TArray<FTransform>> GenerateChunk(const FIntPoint& ChunkIndex);
	[[nodiscard]] int16 GenerateCellContent(const FGridPosition& Pos);

private:
	void GenerateChunk(TArray<TArray<FTransform>>& OutInstances, const FIntPoint& ChunkIndex);
	TArray<TArray<FTransform>> InitResourcesInstances() const;
	
	UGridSubsystem& Grid;
	FRandomStream RngStream;
	FVector2D SeedOffset;
	TArray<FVector2D> ResourcesOffset;
};
