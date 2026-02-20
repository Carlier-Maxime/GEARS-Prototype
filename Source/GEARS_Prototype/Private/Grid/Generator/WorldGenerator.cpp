#include "WorldGenerator.h"

#include "Settings/GridParams.h"

FChunkGenerationResult::FChunkGenerationResult()
{
	ResourcesInstances.Init({}, GridParams::Get().GetResourceRegistry().Num());
}

WorldGenerator::WorldGenerator(const int32 Seed) : ResourceGenerator(Seed), BiomeGenerator(Seed) {}

FChunkGenerationResult WorldGenerator::GenerateChunk(const FChunkIndex& Index) const
{
	FChunkGenerationResult Result;
	GenerateChunk(Result, Index);
	return std::move(Result);
}

void WorldGenerator::GenerateChunk(FChunkGenerationResult& Result, const FChunkIndex& Index) const
{
	for (const auto Local : Index)
	{
		const auto Pos = FWorldGridPos(Index, Local);
		Result.ChunkData.SetBiome(Pos, BiomeGenerator.SampleBiome(Pos));
		const auto [TypeIndex, Transform] = ResourceGenerator.Sample(Pos);
		if (TypeIndex == -1) continue;
		Result.ChunkData.SetResource(Pos, TypeIndex);
		Result.ResourcesInstances[TypeIndex].Add(Transform);
	}
}
