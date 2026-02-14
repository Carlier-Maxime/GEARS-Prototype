#include "WorldGenerator.h"

#include "Settings/GridParams.h"

WorldGenerator::WorldGenerator(UGridSubsystem& Grid, const int32 Seed) :
	Grid(Grid), ResourceGenerator(Seed)
{}

TArray<TArray<FTransform>> WorldGenerator::Generate(const uint16 ChunkRadius) const
{
	auto Instances = InitResourcesInstances();
	for (int16 x=-ChunkRadius; x<=ChunkRadius; ++x)
		for (int16 y=-ChunkRadius; y<=ChunkRadius; ++y)
			GenerateChunk(Instances, {x, y});
	return std::move(Instances);
}

TArray<TArray<FTransform>> WorldGenerator::InitResourcesInstances()
{
	TArray<TArray<FTransform>> ResourcesInstances;
	ResourcesInstances.Init({}, GridParams::Get().GetResourceRegistry().Num());
	return std::move(ResourcesInstances);
}

TArray<TArray<FTransform>> WorldGenerator::GenerateChunk(const FChunkIndex& Index) const
{
	auto Instances = InitResourcesInstances();
	GenerateChunk(Instances, Index);
	return std::move(Instances);
}

void WorldGenerator::GenerateChunk(TArray<TArray<FTransform>>& OutInstances, const FChunkIndex& Index) const
{
	auto& Chunk = Grid.GetChunk(Index);
	if (Index == FIntPoint::ZeroValue) return;
	for (const auto Local : Index)
	{
		const auto Pos = FWorldGridPos(Index, Local);
		const auto [TypeIndex, Transform] = ResourceGenerator.Sample(Pos);
		if (TypeIndex == -1) continue;
		Chunk.SetResource(Pos, TypeIndex);
		OutInstances[TypeIndex].Add(Transform);
	}
}
