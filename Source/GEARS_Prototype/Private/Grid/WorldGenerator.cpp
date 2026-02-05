#include "WorldGenerator.h"

#include "GridSubsystem.h"
#include "Settings/GridParams.h"

constexpr float RngSeedMove = 100000.f;
constexpr float RngResourceMove = 10000.f;

WorldGenerator::WorldGenerator(UGridSubsystem& Grid, const int32 Seed) :
		Grid(Grid), RngStream(Seed), SeedOffset({
			RngStream.FRandRange(-RngSeedMove, RngSeedMove),
			RngStream.FRandRange(-RngSeedMove, RngSeedMove)
		})
{
	const auto Registry = GridParams::Get().GetResourceRegistry();
	ResourcesOffset.Reserve(Registry.Num());
	for (auto i=0; i<Registry.Num(); ++i) ResourcesOffset.Add({
		RngStream.FRandRange(-RngResourceMove, RngResourceMove),
		RngStream.FRandRange(-RngResourceMove, RngResourceMove)
	});
}

void WorldGenerator::Generate(const uint16 ChunkRadius)
{
	for (auto x=-ChunkRadius; x<=ChunkRadius; ++x)
		for (auto y=-ChunkRadius; y<=ChunkRadius; ++y)
			GenerateChunk({x, y});
}

void WorldGenerator::GenerateChunk(const FIntPoint& ChunkIndex)
{
	auto& Chunk = Grid.GetChunk(ChunkIndex);
	auto Pos = FGridPosition::FromChunkIndex(ChunkIndex);
	for (uint32 x=0; x<GridParams::Get().GetChunkSize(); ++x)
	{
		Pos.X += x;
		Pos.Y = ChunkIndex.Y << GridParams::Get().GetChunkShift();
		for (uint32 y=0; y<GridParams::Get().GetChunkSize(); ++y)
		{
			Pos.Y += y;
			const auto Index = GenerateCellContent(Pos);
			if (Index == -1) continue;
			Chunk.SetResource(Pos, Index);
		}
	}
}

int16 WorldGenerator::GenerateCellContent(const FGridPosition& Pos)
{
	for (auto i=0; i<ResourcesOffset.Num(); ++i)
	{
		const auto Resource = GridParams::Get().GetResourceRegistry()[i].LoadSynchronous();
		const auto ResourceOffset = ResourcesOffset[i];
		const auto NoiseValue = (FMath::PerlinNoise2D({
			(Pos.X + SeedOffset.X + ResourceOffset.X) * Resource->NoiseScale,
			(Pos.Y + SeedOffset.Y + ResourceOffset.Y) * Resource->NoiseScale
		}) + 1.0f) * 0.5f;
		if (NoiseValue > Resource->NoiseThreshold) return i;
	}
	return -1;
}
