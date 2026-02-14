#pragma once

#include "Grid/GridMath.h"
#include "Grid/Types/ChunkIndex.h"
#include "Grid/Types/InChunkPos.h"
#include "WorldGridPos.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FWorldGridPos : public FIntPoint
{
	GENERATED_BODY()
	FORCEINLINE FWorldGridPos() = default;
	FORCEINLINE explicit FWorldGridPos(const FVector& WorldPos);
	FORCEINLINE FWorldGridPos(int32 X, int32 Y) : FIntPoint(X, Y) {}
	FORCEINLINE FWorldGridPos(const FChunkIndex& Index, const FInChunkPos& Pos);
	
	FORCEINLINE FVector ToWorld() const;
	FORCEINLINE FVector2D ToWorld2D() const;
	FORCEINLINE FTransform ToTransform() const;
	FORCEINLINE FChunkIndex ToChunkIndex() const;
	FORCEINLINE FInChunkPos ToInChunkPos() const;
	
	static FWorldGridPos StartOf(const FChunkIndex& Index) {return {Index, FInChunkPos::Start};}
};

FWorldGridPos::FWorldGridPos(const FVector& WorldPos) :
	FIntPoint(
		GridMath::Snap(WorldPos.X),
		GridMath::Snap(WorldPos.Y)
	)
{}

FWorldGridPos::FWorldGridPos(const FChunkIndex& Index, const FInChunkPos& Pos) :
	FIntPoint(
	GridMath::ChunkToGrid(Index.X) + Pos.X,
	GridMath::ChunkToGrid(Index.Y) + Pos.Y
	)
{}

FVector FWorldGridPos::ToWorld() const
{
	return FVector(
		GridMath::GridToWorld(X),
		GridMath::GridToWorld(Y),
		0.1f
	);
}

FVector2D FWorldGridPos::ToWorld2D() const
{
	return FVector2D(
		GridMath::GridToWorld(X),
		GridMath::GridToWorld(Y)
	);
}

FTransform FWorldGridPos::ToTransform() const
{
	return FTransform(FQuat::Identity, ToWorld());
}

FChunkIndex FWorldGridPos::ToChunkIndex() const
{
	return {
		static_cast<int16>(GridMath::GridToChunkIndex(X)),
		static_cast<int16>(GridMath::GridToChunkIndex(Y))
	};
}

FInChunkPos FWorldGridPos::ToInChunkPos() const
{
	return FInChunkPos(
		GridMath::GridToInChunk(X),
		GridMath::GridToInChunk(Y)
	);
}

FORCEINLINE uint32 GetTypeHash(const FWorldGridPos& Pos)
{
	return HashCombineFast(GetTypeHash(Pos.X), GetTypeHash(Pos.Y));
}