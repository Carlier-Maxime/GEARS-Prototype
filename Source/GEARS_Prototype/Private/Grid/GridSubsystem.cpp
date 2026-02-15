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
	Generator = new WorldGenerator(Seed);
	Renderer = GetWorld()->SpawnActor<AWorldRenderer>();
	constexpr int16 ChunkRadius = 8;
	for (int16 x=-ChunkRadius; x<=ChunkRadius; ++x)
		for (int16 y=-ChunkRadius; y<=ChunkRadius; ++y)
			CreateChunk(FChunkIndex(x, y));
}

void UGridSubsystem::CreateChunk(const FChunkIndex& Index)
{
	auto Result = Generator->GenerateChunk(Index);
	Chunks.Add(Index, std::move(Result.ChunkData));
	Renderer->AddPlane(Index);
	Renderer->AddResourcesInstances(Index, Result.ResourcesInstances);
}

const FChunkData& UGridSubsystem::GetChunk(const FChunkIndex& Index) const
{
	return Chunks.FindChecked(Index);
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
