// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ChunkData.h"
#include "Types/GridPosition.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridSubsystem.generated.h"

class WorldGenerator;

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGridSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void GenWorld(int32 Seed);

	FChunkData& GetChunk(const FIntPoint& ChunkPos);
	const FChunkData& GetChunk(const FIntPoint& ChunkPos) const;
	FChunkData& GetChunk(const FGridPosition& GridPos);
	const FChunkData& GetChunk(const FGridPosition& GridPos) const;
	
	static FGridPosition WorldToGrid(const FVector& WorldPosition);
	static FVector GridToWorld(const FGridPosition& GridPos);
	
	TMap<FIntPoint, FChunkData> Chunks;
	WorldGenerator* Generator = nullptr;
};
