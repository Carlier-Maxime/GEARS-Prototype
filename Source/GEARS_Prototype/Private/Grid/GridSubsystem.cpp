// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"
#include "Generator/WorldGenerator.h"
#include "Rendering/WorldRenderer.h"
#include "Rendering/WorldRenderBatcher.h"

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
	auto RendererLock = Renderer->Batcher();
	const uint16 ChunkRadius = GridParams::Get().GetMapRadius();
	const int32 Diameter = (ChunkRadius<<1) + 1;
	const int32 ChunksCount = Diameter * Diameter;
	RendererLock.ReserveTasks(ChunksCount);
	ParallelFor(ChunksCount, [this, &RendererLock, Diameter, ChunkRadius](int32 Index)
	{
		const int16 x = (Index % Diameter) - ChunkRadius;
		const int16 y = (Index / Diameter) - ChunkRadius;
		CreateChunk(FChunkIndex(x, y), RendererLock);
	});
}

void UGridSubsystem::CreateChunk(const FChunkIndex& Index, FWorldRenderBatcher& RendererLock)
{
	static FRWLock ChunksLock;
	auto Result = Generator->GenerateChunk(Index);
	if (Index == FIntPoint::ZeroValue) Result.ResourcesInstances.Empty();
	FWriteScopeLock Lock(ChunksLock);
	const auto& Chunk = Chunks.Add(Index, std::move(Result.ChunkData));
	RendererLock.AddChunkTask({Index, std::move(Result.ResourcesInstances), Chunk.GetBiomeMap()});
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
