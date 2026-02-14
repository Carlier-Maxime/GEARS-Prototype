#pragma once

#include "Grid/GridMath.h"
#include "InChunkPos.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FInChunkPos : public FIntPoint
{
	GENERATED_BODY()
	FORCEINLINE FInChunkPos() = default;
	FORCEINLINE FInChunkPos(int32 X, int32 Y) : FIntPoint(X, Y) {}
	FORCEINLINE int16 Flatten() const {return GridMath::FlattenChunk(X, Y);}
	
	static FInChunkPos Start;
	FORCEINLINE static FInChunkPos End()
	{
		return {
			static_cast<int32>(GridParams::Get().GetChunkMask()),
			static_cast<int32>(GridParams::Get().GetChunkMask())
		};
	}
};
