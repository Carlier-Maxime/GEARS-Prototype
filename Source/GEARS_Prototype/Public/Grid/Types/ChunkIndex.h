#pragma once

#include "Grid/GridMath.h"
#include "ChunkIterator.h"
#include "ChunkIndex.generated.h"

struct FWorldGridPos;
struct FInChunkPos;

USTRUCT()
struct FChunkIndex : public FIntPoint
{
	GENERATED_BODY()
	FORCEINLINE FChunkIndex() = default;
	FORCEINLINE FChunkIndex(int16 X, int16 Y) : FIntPoint(X, Y) {}
	
	FORCEINLINE FVector GetCenter() const;
	FORCEINLINE FVector2D GetCenter2D() const;
	
	FORCEINLINE static FChunkIterator begin() {return FChunkIterator::Begin();}
	FORCEINLINE static FChunkIterator end() {return FChunkIterator::End();}
};

FVector FChunkIndex::GetCenter() const
{
	return {
		GridMath::ChunkToCenter(X),
		GridMath::ChunkToCenter(Y),
		0
	};
}

FVector2D FChunkIndex::GetCenter2D() const
{
	return {
		GridMath::ChunkToCenter(X),
		GridMath::ChunkToCenter(Y)
	};
}
