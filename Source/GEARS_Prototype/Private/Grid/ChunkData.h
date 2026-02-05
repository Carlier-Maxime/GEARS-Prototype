#pragma once

#include "ChunkData.generated.h"

USTRUCT()
struct FChunkData
{
	GENERATED_BODY()
	FChunkData();
	void SetResource(const FIntPoint& Pos, int16 ResourceIndex);
private:
	TArray<int16> ResourceMap;
};
