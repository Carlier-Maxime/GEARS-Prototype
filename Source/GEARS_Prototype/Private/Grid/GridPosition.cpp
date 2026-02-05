#include "GridPosition.h"

#include "Settings/GridParams.h"

FGridPosition::FGridPosition(const FVector& WorldPos) :
	Pos(
	FMath::FloorToInt(WorldPos.X * GridParams::Get().GetInvCellSize()),
	FMath::FloorToInt(WorldPos.Y * GridParams::Get().GetInvCellSize())
	)
{}

FVector FGridPosition::ToWorld() const
{
	return FVector(
		(Pos.X + 0.5f) * GridParams::Get().GetCellSize(),
		(Pos.Y + 0.5f) * GridParams::Get().GetCellSize(),
		0.1f
	);
}

FVector2D FGridPosition::ToWorld2D() const
{
	return FVector2D(
		(Pos.X + 0.5f) * GridParams::Get().GetCellSize(),
		(Pos.Y + 0.5f) * GridParams::Get().GetCellSize()
	);
}

FIntPoint FGridPosition::GetChunkIndex() const
{
	return {
		Pos.X >> GridParams::Get().GetChunkShift(),
		Pos.Y >> GridParams::Get().GetChunkShift()
	};
}

FIntPoint FGridPosition::GetLocalPos() const
{
	return {
		Pos.X & GridParams::Get().GetChunkMask(),
		Pos.Y & GridParams::Get().GetChunkMask()
	};
}
