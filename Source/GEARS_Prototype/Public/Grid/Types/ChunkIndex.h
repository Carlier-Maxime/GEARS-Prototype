#pragma once

#include "Grid/GridMath.h"
#include "ChunkIterator.h"
#include "Grid/Types/InBiomeChunkPos.h"
#include "Grid/Types/BiomeChunkIndex.h"
#include "ChunkIndex.generated.h"

USTRUCT()
struct FChunkIndex : public FIntPoint
{
	GENERATED_BODY()
	FORCEINLINE FChunkIndex() = default;
	FORCEINLINE FChunkIndex(int16 X, int16 Y) : FIntPoint(X, Y) {}
	FORCEINLINE FChunkIndex(const FBiomeChunkIndex& Index, const FInBiomeChunkPos& Pos);
	
	FORCEINLINE FVector GetCenter() const;
	FORCEINLINE FVector2D GetCenter2D() const;
	FORCEINLINE FBiomeChunkIndex ToBiomeChunkIndex() const;
	FORCEINLINE FInBiomeChunkPos ToInBiomeChunkPos() const;
	
	FORCEINLINE static FChunkIterator begin() {return FChunkIterator::Begin();}
	FORCEINLINE static FChunkIterator end() {return FChunkIterator::End();}
	
	static FChunkIndex StartOf(const FBiomeChunkIndex& Index) {return {Index, FInBiomeChunkPos::Start};}
};

inline FChunkIndex::FChunkIndex(const FBiomeChunkIndex& Index, const FInBiomeChunkPos& Pos) :
	FIntPoint(
		GridMath::BiomeChunkToChunk(Index.X) + Pos.X,
		GridMath::BiomeChunkToChunk(Index.Y) + Pos.Y
	)
{}

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

inline FBiomeChunkIndex FChunkIndex::ToBiomeChunkIndex() const
{
	return {
		static_cast<int16>(GridMath::ChunkToBiomeChunkIndex(X)),
		static_cast<int16>(GridMath::ChunkToBiomeChunkIndex(Y))
	};
}

inline FInBiomeChunkPos FChunkIndex::ToInBiomeChunkPos() const
{
	return {
		GridMath::ChunkToInBiomeChunk(X),
		GridMath::ChunkToInBiomeChunk(Y)
	};
}
