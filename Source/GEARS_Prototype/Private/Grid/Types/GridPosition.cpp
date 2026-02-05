#include "GridPosition.h"

#include "Settings/GridParams.h"

FGridPosition::FGridPosition(const FVector& WorldPos) :
	FGridPosition(
	FMath::FloorToInt(WorldPos.X * GridParams::Get().GetInvCellSize()),
	FMath::FloorToInt(WorldPos.Y * GridParams::Get().GetInvCellSize())
	)
{}

FGridPosition::FGridPosition(const int32 X, const int32 Y) : X(X), Y(Y) {}

FGridPosition::FGridPosition(const FIntPoint& Pos) : FGridPosition(Pos.X, Pos.Y) {}

FGridPosition FGridPosition::FromChunkIndex(const FIntPoint& ChunkIndex)
{
	return FGridPosition(
		ChunkIndex.X << GridParams::Get().GetChunkShift(),
		ChunkIndex.Y << GridParams::Get().GetChunkShift()
	);
}

FVector FGridPosition::ToWorld() const
{
	return FVector(
		(X + 0.5f) * GridParams::Get().GetCellSize(),
		(Y + 0.5f) * GridParams::Get().GetCellSize(),
		0.1f
	);
}

FVector2D FGridPosition::ToWorld2D() const
{
	return FVector2D(
		(X + 0.5f) * GridParams::Get().GetCellSize(),
		(Y + 0.5f) * GridParams::Get().GetCellSize()
	);
}

FTransform FGridPosition::ToTransform() const
{
	return FTransform(FQuat::Identity, ToWorld());
}

FIntPoint FGridPosition::GetChunkIndex() const
{
	return {
		X >> GridParams::Get().GetChunkShift(),
		Y >> GridParams::Get().GetChunkShift()
	};
}

FIntPoint FGridPosition::GetLocalPos() const
{
	return {
		X & static_cast<int32>(GridParams::Get().GetChunkMask()),
		Y & static_cast<int32>(GridParams::Get().GetChunkMask())
	};
}

FIntPoint FGridPosition::GetGridPos() const
{
	return {X, Y};
}
