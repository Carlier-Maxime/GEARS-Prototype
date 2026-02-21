#pragma once

#include "Grid/GridMath.h"
#include "InBiomeChunkPos.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FInBiomeChunkPos : public FIntPoint
{
	GENERATED_BODY()
	FORCEINLINE FInBiomeChunkPos() = default;
	FORCEINLINE FInBiomeChunkPos(int32 X, int32 Y) : FIntPoint(X, Y) {}
	FORCEINLINE int16 Flatten() const {return GridMath::FlattenBiomeChunk(X, Y);}
	
	static FInBiomeChunkPos Start;
	FORCEINLINE static FInBiomeChunkPos End()
	{
		return {
			static_cast<int32>(GridParams::Get().GetBiomeChunkMask()),
			static_cast<int32>(GridParams::Get().GetBiomeChunkMask())
		};
	}
	FORCEINLINE static FInBiomeChunkPos PostEnd()
	{
		return {
			static_cast<int32>(0),
			static_cast<int32>(GridParams::Get().GetBiomeChunkFactor())
		};
	}
};
