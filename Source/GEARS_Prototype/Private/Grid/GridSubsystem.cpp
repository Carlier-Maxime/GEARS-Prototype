// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"
#include "Types/GridPosition.h"
#include "WorldGenerator.h"
#include "WorldRenderer.h"

void UGridSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGridSubsystem::Deinitialize()
{
	Super::Deinitialize();
	if (Renderer) Renderer->Destroy();
	delete Generator;
}

void UGridSubsystem::GenWorld(const int32 Seed)
{
	Generator = new WorldGenerator(*this, Seed);
	Renderer = GetWorld()->SpawnActor<AWorldRenderer>();
	Renderer->UpdateResourcesInstances(std::move(Generator->Generate(8)));
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
