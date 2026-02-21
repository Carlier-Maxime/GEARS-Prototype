#pragma once
#include "BiomeIndexMapScopedLock.h"

class FBiomeTextureMapper
{
public:
	FBiomeTextureMapper();
	explicit FBiomeTextureMapper(UTexture2DArray& InAtlas, UTexture2DArray& InIndexMap);
	FBiomeIndexMapScopedLock Lock() const;

private:
	static void ConfigureT2A(UTexture2DArray& Texture, uint16 Size, uint16 Depth, const EPixelFormat& Format);
	void FillAtlas();
	
	UTexture2DArray& Atlas;
	UTexture2DArray& IndexMap;
};
