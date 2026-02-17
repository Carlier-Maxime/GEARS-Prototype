#include "ChunkData.h"

#include "Settings/GridParams.h"

FChunkData::FChunkData()
{
	const auto Size = GridParams::Get().GetChunkSizeSquared();
	ResourceMap.Init(-1, Size);
	BiomeMap.Init(-1, Size);
}

void FChunkData::SetResource(const FInChunkPos& Pos, const int16 ResourceIndex)
{
	ResourceMap[Pos.Flatten()] = ResourceIndex;
}

void FChunkData::SetBiome(const FInChunkPos& Pos, int8 Biome)
{
	BiomeMap[Pos.Flatten()] = Biome;
}
