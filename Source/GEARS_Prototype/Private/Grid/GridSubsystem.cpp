// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"

#include "GridPosition.h"

void UGridSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGridSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FChunkData& UGridSubsystem::GetChunk(const FIntPoint& ChunkPos)
{
	return Chunks.FindOrAdd(ChunkPos);
}

const FChunkData& UGridSubsystem::GetChunk(const FIntPoint& ChunkPos) const
{
	return Chunks.FindChecked(ChunkPos);
}

FChunkData& UGridSubsystem::GetChunk(const FGridPosition& GridPos)
{
	return GetChunk(GridPos.GetChunkIndex());
}

const FChunkData& UGridSubsystem::GetChunk(const FGridPosition& GridPos) const
{
	return GetChunk(GridPos.GetChunkIndex());
}

FGridPosition UGridSubsystem::WorldToGrid(const FVector& WorldPosition)
{
	return FGridPosition(WorldPosition);
}

FVector UGridSubsystem::GridToWorld(const FGridPosition& GridPos)
{
	return GridPos.ToWorld();
}
