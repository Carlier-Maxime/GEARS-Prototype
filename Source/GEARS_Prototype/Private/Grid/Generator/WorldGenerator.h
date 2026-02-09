#pragma once

#include "Grid/Generator/BaseGenerator.h"
#include "../GridSubsystem.h"

class WorldGenerator : public BaseGenerator
{
public:
	WorldGenerator(UGridSubsystem& Grid, int32 Seed);
	[[nodiscard]] TArray<TArray<FTransform>> Generate(const uint16 ChunkRadius) const;
	[[nodiscard]] TArray<TArray<FTransform>> GenerateChunk(const FIntPoint& ChunkIndex) const;

private:
	void GenerateChunk(TArray<TArray<FTransform>>& OutInstances, const FIntPoint& ChunkIndex) const;
	static TArray<TArray<FTransform>> InitResourcesInstances();
	
	UGridSubsystem& Grid;
};
