#include "WorldGenerator.h"

#include "Settings/GridParams.h"

WorldGenerator::WorldGenerator(UGridSubsystem& Grid, const int32 Seed) :
		BaseGenerator(Seed), Grid(Grid)
{}

TArray<TArray<FTransform>> WorldGenerator::Generate(const uint16 ChunkRadius) const
{
	auto Instances = InitResourcesInstances();
	for (auto x=-ChunkRadius; x<=ChunkRadius; ++x)
		for (auto y=-ChunkRadius; y<=ChunkRadius; ++y)
			GenerateChunk(Instances, {x, y});
	return std::move(Instances);
}

TArray<TArray<FTransform>> WorldGenerator::InitResourcesInstances()
{
	TArray<TArray<FTransform>> ResourcesInstances;
	ResourcesInstances.Init({}, GridParams::Get().GetResourceRegistry().Num());
	return std::move(ResourcesInstances);
}

TArray<TArray<FTransform>> WorldGenerator::GenerateChunk(const FIntPoint& ChunkIndex) const
{
	auto Instances = InitResourcesInstances();
	GenerateChunk(Instances, ChunkIndex);
	return std::move(Instances);
}

void WorldGenerator::GenerateChunk(TArray<TArray<FTransform>>& OutInstances, const FIntPoint& ChunkIndex) const
{
	auto& Chunk = Grid.GetChunk(ChunkIndex);
	if (ChunkIndex == FIntPoint::ZeroValue) return;
	auto Pos = FGridPosition::FromChunkIndex(ChunkIndex);
	for (uint32 x=0; x<GridParams::Get().GetChunkSize(); ++x)
	{
		++Pos.X;
		Pos.Y = ChunkIndex.Y << GridParams::Get().GetChunkShift();
		for (uint32 y=0; y<GridParams::Get().GetChunkSize(); ++y)
		{
			++Pos.Y;
			const auto SpawnData = SampleResourceAtPosition(Pos);
			const auto Index = SpawnData.ResourceTypeIndex;
			if (Index == -1) continue;
			Chunk.SetResource(Pos, SpawnData.ResourceTypeIndex);
			OutInstances[Index].Add(SpawnData.Transform);
		}
	}
}
