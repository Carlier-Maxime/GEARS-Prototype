#pragma once

struct FPixelWriteContext
{
	uint8* SliceData;
	int32 DataIndex;
	int32 PixelOffset;
};

class FTextureScopedLock
{
public:
	FTextureScopedLock(UTexture2DArray& InTexture, uint16 InOctetsPerPixel);
	virtual ~FTextureScopedLock();
	
	void UpdateSlice(int32 SliceIndex, const TFunctionRef<void(FPixelWriteContext)>& UpdateSliceDataAt) const;
	
protected:
	UTexture2DArray& Texture;
	uint64 PixelsPerSlice;
	uint64 OctetsPerSlice;
	uint16 OctetsPerPixel;
	uint8* RawData;
};
