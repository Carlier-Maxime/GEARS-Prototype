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

void FChunkData::SetBiome(const FInChunkPos& Pos, uint8 Biome)
{
	BiomeMap[Pos.Flatten()] = Biome;
}

const FResourceState* FChunkData::GetResourceState(const FInChunkPos& Pos) const
{
	const auto Index = Pos.Flatten();
	if (IsDynamic[Index]) return &DynamicStates.FindChecked(Index);
	const auto Resource = ResourceMap[Index];
	if (Resource == FResourceRegistry::INVALID_INDEX) return nullptr;
	return &GridParams::Get().GetResourceRegistry()[Resource].State;
}

FResourceState* FChunkData::GetMutableResourceState(const FInChunkPos& Pos)
{
	const auto Index = Pos.Flatten();
	if (IsDynamic[Index]) return &DynamicStates.FindChecked(Index);
	const auto Resource = ResourceMap[Index];
	if (Resource == FResourceRegistry::INVALID_INDEX) return nullptr;
	IsDynamic[Index] = true;
	return &DynamicStates.Emplace(Index, GridParams::Get().GetResourceRegistry()[Resource].State);
}

void FChunkData::ResetResourceState(const FInChunkPos& Pos)
{
	const auto Index = Pos.Flatten();
	if (IsDynamic[Index]) DynamicStates.Remove(Index);
	IsDynamic[Index] = false;
}
