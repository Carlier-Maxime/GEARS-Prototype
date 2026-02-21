#pragma once
#include "TextureScopedLock.h"
#include "Grid/Types.h"

class FBiomeIndexMapScopedLock : public FTextureScopedLock
{
public:
	explicit FBiomeIndexMapScopedLock(UTexture2DArray& InIndexMap);

	static size_t GetSliceIndex(const FBiomeChunkIndex& Index);
	FORCEINLINE void UpdateBiomeChunkPart(const FBiomeChunkIndex& Index, const FInBiomeChunkPos& Pos, const TArray<uint8>& Biomes) const
	{
		UpdateBiomeChunkPart(GetSliceIndex(Index), Pos, Biomes);
	}
	void UpdateBiomeChunkPart(const int64 SliceIndex, const FInBiomeChunkPos& Pos, const TArray<uint8>& Biomes) const;
};
