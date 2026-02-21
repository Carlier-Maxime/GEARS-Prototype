#pragma once

#include "BiomeChunkIndex.generated.h"

USTRUCT()
struct FBiomeChunkIndex : public FIntPoint
{
	GENERATED_BODY()
	
	FORCEINLINE FBiomeChunkIndex() = default;
	FORCEINLINE FBiomeChunkIndex(int16 X, int16 Y) : FIntPoint(X, Y) {}
};
