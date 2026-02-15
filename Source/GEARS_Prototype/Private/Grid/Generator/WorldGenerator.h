#pragma once

#include "../GridSubsystem.h"
#include "Grid/Generator/ResourceGenerator.h"

struct FChunkGenerationResult
{
	FChunkGenerationResult();
	TArray<TArray<FTransform>> ResourcesInstances;
	FChunkData ChunkData;
};

class WorldGenerator
{
public:
	virtual ~WorldGenerator() = default;
	WorldGenerator(int32 Seed);
	[[nodiscard]] FChunkGenerationResult GenerateChunk(const FChunkIndex& Index) const;

private:
	void GenerateChunk(FChunkGenerationResult& Result, const FChunkIndex& Index) const;
	
	ResourceGenerator ResourceGenerator;
};
