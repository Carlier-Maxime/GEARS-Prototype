#include "BiomeTextureMapper.h"

#include "Engine/Texture2DArray.h"
#include "Settings/GridParams.h"

FBiomeTextureMapper::FBiomeTextureMapper() : FBiomeTextureMapper(*GridParams::Get().GetBiomeAtlas()) {}

FBiomeTextureMapper::FBiomeTextureMapper(UTexture2DArray& InAtlas) : Atlas(InAtlas)
{
	const auto& Params = GridParams::Get();
	const auto Size = Params.GetChunkSize();
	constexpr auto Depth = 1024;
	constexpr auto Format = PF_B8G8R8A8;
	
	auto* PlatformData = new FTexturePlatformData();
	PlatformData->SizeX = Size;
	PlatformData->SizeY = Size;
	PlatformData->SetNumSlices(Depth);
	PlatformData->PixelFormat = Format;
	
	auto* Mip = new FTexture2DMipMap();
	PlatformData->Mips.Add(Mip);
	Mip->SizeX = Size;
	Mip->SizeY = Size;
	Mip->SizeZ = Depth;

	const long DataSize = Size * Size * Depth * GPixelFormats[Format].BlockBytes;
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	Mip->BulkData.Realloc(DataSize);
	Mip->BulkData.Unlock();
	
	Atlas.ReleaseResource();
	Atlas.SetPlatformData(PlatformData);
	Atlas.SRGB = true;
	Atlas.NeverStream = true;
	Atlas.CompressionSettings = TC_Default;
	Atlas.Filter = TF_Nearest;
	Atlas.UpdateResource();
}

FBiomeAtlasScopedLock FBiomeTextureMapper::Lock() const
{
	const auto* Data = Atlas.GetPlatformData();
	const auto PixelsPerSlice = Data->SizeX * Data->SizeY;
	return {Atlas, PixelsPerSlice, PixelsPerSlice * 4};
}
