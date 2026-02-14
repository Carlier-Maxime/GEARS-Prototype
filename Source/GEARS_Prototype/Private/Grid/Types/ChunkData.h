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
private:
	TArray<int16> ResourceMap;
};

void FChunkData::SetResource(const FWorldGridPos& GridPos, const int16 ResourceIndex)
{
	return SetResource(GridPos.ToInChunkPos(), ResourceIndex);
}