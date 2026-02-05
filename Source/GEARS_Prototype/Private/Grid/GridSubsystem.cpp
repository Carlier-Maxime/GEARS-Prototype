// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"

#include "Settings/GridParams.h"

void UGridSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGridSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FChunkData& UGridSubsystem::GetChunk(const FIntPoint& Position)
{
	return Chunks.FindOrAdd(Position);
}

const FChunkData& UGridSubsystem::GetChunk(const FIntPoint& Position) const
{
	return Chunks.FindChecked(Position);
}

FIntPoint UGridSubsystem::WorldToGrid(const FVector& WorldPosition)
{
	return FIntPoint(
		FMath::FloorToInt(WorldPosition.X * GridParams::GetInvCellSize()),
		FMath::FloorToInt(WorldPosition.Y * GridParams::GetInvCellSize())
	);
}

FVector UGridSubsystem::GridToWorld(const FIntPoint& GridPosition)
{
	return FVector(
		(GridPosition.X + 0.5f) * GridParams::GetCellSize(),
		(GridPosition.Y + 0.5f) * GridParams::GetCellSize(),
		0.1f
	);
}
