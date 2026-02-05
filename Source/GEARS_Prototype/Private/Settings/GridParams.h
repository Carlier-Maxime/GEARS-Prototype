#pragma once

#include "Data/ResourceType.h"

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
	const TArray<TSoftObjectPtr<UResourceType>>& GetResourceRegistry() {return ResourceRegistry;}
private:
	GridParams() = default;
	friend class UGridSettings;
	float CellSize;
	float InvCellSize;
	uint32 ChunkSize;
	uint32 ChunkSizeSquared;
	TArray<TSoftObjectPtr<UResourceType>> ResourceRegistry;
};
