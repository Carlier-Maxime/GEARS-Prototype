#pragma once

#include "Data/ResourceType.h"

class GridParams
{
public:
	static float GetCellSize() { return CellSize; }
	static float GetInvCellSize() { return InvCellSize; }
	static uint32 GetChunkSize() { return ChunkSize; }
	static uint32 GetChunkSizeSquared() { return ChunkSizeSquared; }
	static const TArray<TSoftObjectPtr<UResourceType>>& GetResourceRegistry() {return ResourceRegistry;}
private:
	friend class UGridSettings;
	inline static float CellSize;
	inline static float InvCellSize;
	inline static uint32 ChunkSize;
	inline static uint32 ChunkSizeSquared;
	inline static TArray<TSoftObjectPtr<UResourceType>> ResourceRegistry;
};
