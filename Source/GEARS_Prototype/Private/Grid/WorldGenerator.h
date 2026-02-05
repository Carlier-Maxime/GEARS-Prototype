#pragma once
#include "GridSubsystem.h"

class WorldGenerator
{
public:
	WorldGenerator(UGridSubsystem& Grid, int32 Seed);
	void Generate(const uint16 ChunkRadius);
	void GenerateChunk(const FIntPoint& ChunkIndex);
	[[nodiscard]] int16 GenerateCellContent(const FGridPosition& Pos);
private:
	UGridSubsystem& Grid;
	FRandomStream RngStream;
	FVector2D SeedOffset;
	TArray<FVector2D> ResourcesOffset;
};
