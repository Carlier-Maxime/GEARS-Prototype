// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"
#include "Generator/WorldGenerator.h"
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

FChunkData& UGridSubsystem::GetChunk(const FChunkIndex& Index)
{
	return Chunks.FindOrAdd(Index);
}

const FChunkData& UGridSubsystem::GetChunk(const FChunkIndex& Index) const
{
	return Chunks.FindChecked(Index);
}

FChunkData& UGridSubsystem::GetChunk(const FWorldGridPos& GridPos)
{
	return GetChunk(GridPos.ToChunkIndex());
}

const FChunkData& UGridSubsystem::GetChunk(const FWorldGridPos& GridPos) const
{
	return GetChunk(GridPos.ToChunkIndex());
}

FWorldGridPos UGridSubsystem::WorldToGrid(const FVector& WorldPosition)
{
	return FWorldGridPos(WorldPosition);
}

FVector UGridSubsystem::GridToWorld(const FWorldGridPos& GridPos)
{
	return GridPos.ToWorld();
}
