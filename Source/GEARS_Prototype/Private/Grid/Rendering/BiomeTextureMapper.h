#pragma once
#include "BiomeAtlasScopedLock.h"

class FBiomeTextureMapper
{
public:
	FBiomeTextureMapper();
	explicit FBiomeTextureMapper(UTexture2DArray& InAtlas);
	FBiomeAtlasScopedLock Lock() const;
private:
	UTexture2DArray& Atlas;
};
