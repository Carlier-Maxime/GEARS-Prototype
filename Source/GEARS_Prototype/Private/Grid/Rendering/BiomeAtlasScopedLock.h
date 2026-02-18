#pragma once

class FBiomeAtlasScopedLock
{
public:
	FBiomeAtlasScopedLock(UTexture2DArray& InAtlas, int64 InPixelsPerSlice, int64 InOctetsPerSlice);
	~FBiomeAtlasScopedLock();
	
	void UpdateSliceFromBiomeTypes(int32 SliceIndex, const TArray<int8>& BiomeTypes) const;
private:
	UTexture2DArray& Atlas;
	int64 PixelsPerSlice;
	int64 OctetsPerSlice;
	uint8* RawData;
};
