#pragma once
#include "Grid/Types.h"

#include "ChunkData.generated.h"

USTRUCT()
struct FChunkData
{
	GENERATED_BODY()
	FChunkData();
	void SetResource(const FInChunkPos& Pos, int16 ResourceIndex);
	FORCEINLINE void SetResource(const FWorldGridPos& GridPos, int16 ResourceIndex);
	void SetBiome(const FInChunkPos& Pos, uint8 Biome);
	FORCEINLINE void SetBiome(const FWorldGridPos& GridPos, uint8 Biome);
	FORCEINLINE const TArray<uint8>& GetBiomeMap() const {return BiomeMap;}
private:
	TArray<int16> ResourceMap;
	TArray<uint8> BiomeMap;
};

void FChunkData::SetResource(const FWorldGridPos& GridPos, const int16 ResourceIndex)
{
	return SetResource(GridPos.ToInChunkPos(), ResourceIndex);
}

inline void FChunkData::SetBiome(const FWorldGridPos& GridPos, const uint8 Biome)
{
	return SetBiome(GridPos.ToInChunkPos(), Biome);
}