#pragma once

#include "GridPosition.generated.h"

USTRUCT()
struct FGridPosition
{
	GENERATED_BODY()
	FGridPosition() = default;
	explicit FGridPosition(const FVector& WorldPos);
	FVector ToWorld() const;
	FVector2D ToWorld2D() const;
	FIntPoint GetChunkIndex() const;
	FIntPoint GetLocalPos() const;
	FIntPoint Pos = FIntPoint::ZeroValue;
};
