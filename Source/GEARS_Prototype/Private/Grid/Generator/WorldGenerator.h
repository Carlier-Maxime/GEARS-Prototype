#pragma once

#include "BaseGenerator.h"
#include "../GridSubsystem.h"

class WorldGenerator : public BaseGenerator
{
public:
	WorldGenerator(UGridSubsystem& Grid, int32 Seed);
	[[nodiscard]] TArray<TArray<FTransform>> Generate(const uint16 ChunkRadius);
	[[nodiscard]] TArray<TArray<FTransform>> GenerateChunk(const FIntPoint& ChunkIndex);

private:
	void GenerateChunk(TArray<TArray<FTransform>>& OutInstances, const FIntPoint& ChunkIndex);
	TArray<TArray<FTransform>> InitResourcesInstances() const;
	
	UGridSubsystem& Grid;
};
