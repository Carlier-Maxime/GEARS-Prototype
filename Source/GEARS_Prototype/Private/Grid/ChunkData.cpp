#include "ChunkData.h"

#include "Settings/GridParams.h"

FChunkData::FChunkData()
{
	ResourceMap.Init(-1, GridParams::GetChunkSizeSquared());
}

void FChunkData::SetResource(const FIntPoint& Pos, const int16 ResourceIndex)
{
	ResourceMap[Pos.X + Pos.Y * GridParams::GetChunkSize()] = ResourceIndex;
}
