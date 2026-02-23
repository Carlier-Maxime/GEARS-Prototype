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
	const auto& BiomeRegistry = GridParams::Get().GetBiomeRegistry();
	for (const auto Local : Index)
	{
		const auto Pos = FWorldGridPos(Index, Local);
		const auto BiomeIndex = BiomeGenerator.SampleBiome(Pos);
		Result.ChunkData.SetBiome(Pos, BiomeIndex);
		if (BiomeIndex == BiomeRegistry.INVALID_INDEX) continue;
		const auto [TypeIndex, Transform] = ResourceGenerator.Sample(Pos, BiomeRegistry[BiomeIndex].Resources);
		if (TypeIndex == -1) continue;
		Result.ChunkData.SetResource(Pos, TypeIndex);
		Result.ResourcesInstances[TypeIndex].Add(Transform);
	}
}
