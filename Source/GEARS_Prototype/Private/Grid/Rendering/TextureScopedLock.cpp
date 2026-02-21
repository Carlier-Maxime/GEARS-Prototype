#include "TextureScopedLock.h"

#include "Engine/Texture2DArray.h"

FTextureScopedLock::FTextureScopedLock(UTexture2DArray& InTexture, const uint16 InOctetsPerPixel) :
	Texture(InTexture),
	PixelsPerSlice(Texture.GetPlatformData()->SizeX * Texture.GetPlatformData()->SizeY),
	OctetsPerSlice(PixelsPerSlice * InOctetsPerPixel),
	OctetsPerPixel(InOctetsPerPixel),
	RawData(static_cast<uint8*>(Texture.GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE)))
{}

FTextureScopedLock::~FTextureScopedLock()
{
	Texture.GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture.UpdateResource();
}

void FTextureScopedLock::UpdateSlice(const int32 SliceIndex, const TFunctionRef<void(FPixelWriteContext)>& UpdateSliceDataAt) const
{
	if (!RawData) return;
	auto* SliceData = RawData + SliceIndex * OctetsPerSlice;
	for (int32 i = 0; i < PixelsPerSlice; ++i) UpdateSliceDataAt({SliceData, i, i * OctetsPerPixel});
}
