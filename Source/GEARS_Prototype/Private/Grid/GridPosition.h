#pragma once

#include "GridPosition.generated.h"

USTRUCT()
struct FGridPosition
{
	GENERATED_BODY()
	FGridPosition() = default;
	explicit FGridPosition(const FVector& WorldPos);
	static FGridPosition FromChunkIndex(const FIntPoint& ChunkIndex);
	FVector ToWorld() const;
	FVector2D ToWorld2D() const;
	FIntPoint GetChunkIndex() const;
	FIntPoint GetLocalPos() const;
	FIntPoint GetGridPos() const;
	int32 X = 0;
	int32 Y = 0;
private:
	explicit FGridPosition(const FIntPoint& Pos);
	explicit FGridPosition(int32 X, int32 Y);
};
