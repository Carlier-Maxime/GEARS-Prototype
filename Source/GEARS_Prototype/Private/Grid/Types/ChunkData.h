#pragma once
#include "Grid/Types/GridPosition.h"

#include "ChunkData.generated.h"

USTRUCT()
struct FChunkData
{
	GENERATED_BODY()
	FChunkData();
	void SetResource(const FIntPoint& LocalPos, int16 ResourceIndex);
	void SetResource(const FGridPosition& GridPos, int16 ResourceIndex);
private:
	TArray<int16> ResourceMap;
};
