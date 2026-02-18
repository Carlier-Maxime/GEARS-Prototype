// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"
#include "Generator/WorldGenerator.h"
#include "Rendering/WorldRenderer.h"
#include "Rendering/WorldRenderScopedLock.h"

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
	auto RendererLock = Renderer->Lock();
	constexpr int16 ChunkRadius = 8;
	for (int16 x=-ChunkRadius; x<=ChunkRadius; ++x)
		for (int16 y=-ChunkRadius; y<=ChunkRadius; ++y)
			CreateChunk(FChunkIndex(x, y), RendererLock);
}

void UGridSubsystem::CreateChunk(const FChunkIndex& Index, FWorldRenderScopedLock& RendererLock)
{
	auto Result = Generator->GenerateChunk(Index);
	const auto& Chunk = Chunks.Add(Index, std::move(Result.ChunkData));
	RendererLock.AddPlane(Index, Chunk.GetBiomeMap());
	RendererLock.AddResourcesInstances(Index, Result.ResourcesInstances);
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
