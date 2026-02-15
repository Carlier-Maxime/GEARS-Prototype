#include "WorldGenerator.h"

#include "Settings/GridParams.h"

FChunkGenerationResult::FChunkGenerationResult()
{
	ResourcesInstances.Init({}, GridParams::Get().GetResourceRegistry().Num());
}

WorldGenerator::WorldGenerator(const int32 Seed) : ResourceGenerator(Seed) {}

FChunkGenerationResult WorldGenerator::GenerateChunk(const FChunkIndex& Index) const
{
	FChunkGenerationResult Result;
	GenerateChunk(Result, Index);
	return std::move(Result);
}

void WorldGenerator::GenerateChunk(FChunkGenerationResult& Result, const FChunkIndex& Index) const
{
	if (Index == FIntPoint::ZeroValue) return;
	for (const auto Local : Index)
	{
		const auto Pos = FWorldGridPos(Index, Local);
		const auto [TypeIndex, Transform] = ResourceGenerator.Sample(Pos);
		if (TypeIndex == -1) continue;
		Result.ChunkData.SetResource(Pos, TypeIndex);
		Result.ResourcesInstances[TypeIndex].Add(Transform);
	}
}
