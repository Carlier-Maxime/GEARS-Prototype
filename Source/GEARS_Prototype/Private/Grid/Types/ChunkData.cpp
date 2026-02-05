#include "ChunkData.h"

#include "Settings/GridParams.h"

FChunkData::FChunkData()
{
	ResourceMap.Init(-1, GridParams::Get().GetChunkSizeSquared());
}

void FChunkData::SetResource(const FIntPoint& LocalPos, const int16 ResourceIndex)
{
	ResourceMap[LocalPos.X + LocalPos.Y * GridParams::Get().GetChunkSize()] = ResourceIndex;
}

void FChunkData::SetResource(const FGridPosition& GridPos, const int16 ResourceIndex)
{
	return SetResource(GridPos.GetLocalPos(), ResourceIndex);
}
