#pragma once

#include "Data/ResourceType.h"

class UBiomeType;

class GridParams
{
public:
	static GridParams& Get()
	{
		static GridParams Instance;
		return Instance;
	}
	
	float GetCellSize() { return CellSize; }
	float GetInvCellSize() { return InvCellSize; }
	uint32 GetChunkSize() { return ChunkSize; }
	uint32 GetChunkSizeSquared() { return ChunkSizeSquared; }
	uint32 GetChunkShift() { return ChunkShift; }
	uint32 GetChunkMask() { return ChunkMask; }
	const TArray<TSoftObjectPtr<UResourceType>>& GetResourceRegistry() {return ResourceRegistry;}
	const TArray<TSoftObjectPtr<UBiomeType>>& GetBiomeRegistry() {return BiomeRegistry;}
private:
	GridParams() = default;
	friend class UGridSettings;
	float CellSize;
	float InvCellSize;
	uint32 ChunkSize;
	uint32 ChunkSizeSquared;
	uint32 ChunkShift;
	uint32 ChunkMask;
	TArray<TSoftObjectPtr<UResourceType>> ResourceRegistry;
	TArray<TSoftObjectPtr<UBiomeType>> BiomeRegistry;
};
