#include "BiomeTextureMapper.h"

#include "Engine/Texture2DArray.h"
#include "Settings/GridParams.h"

FBiomeTextureMapper::FBiomeTextureMapper() : FBiomeTextureMapper(*GridParams::Get().GetBiomeAtlas()) {}

FBiomeTextureMapper::FBiomeTextureMapper(UTexture2DArray& InAtlas) : Atlas(InAtlas)
{
	const auto& Params = GridParams::Get();
	Atlas.ReleaseResource();
	Atlas.Source.Init(0, 0, 0, 0, TSF_Invalid);
	Atlas.Source.Init(Params.GetChunkSize(), Params.GetChunkSize(), 1024, 1, TSF_BGRA8);
	Atlas.SRGB = true;
	Atlas.NeverStream = true;
	Atlas.CompressionSettings = TC_Default;
	Atlas.Filter = TF_Nearest;
	Atlas.UpdateResource();
	Atlas.FinishCachePlatformData();
}

FBiomeAtlasScopedLock FBiomeTextureMapper::Lock() const
{
	const auto PixelsPerSlice = Atlas.Source.GetSizeX() * Atlas.Source.GetSizeY();
	return {Atlas, PixelsPerSlice, PixelsPerSlice * 4};
}
