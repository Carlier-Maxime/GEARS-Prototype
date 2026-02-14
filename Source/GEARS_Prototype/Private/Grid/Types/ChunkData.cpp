#include "ChunkData.h"

#include "Settings/GridParams.h"

FChunkData::FChunkData()
{
	ResourceMap.Init(-1, GridParams::Get().GetChunkSizeSquared());
}

void FChunkData::SetResource(const FInChunkPos& Pos, const int16 ResourceIndex)
{
	ResourceMap[Pos.Flatten()] = ResourceIndex;
}
