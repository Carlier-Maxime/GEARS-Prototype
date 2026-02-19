// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ChunkData.h"
#include "Grid/Types.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridSubsystem.generated.h"

class FWorldRenderBatcher;
class AWorldRenderer;
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
	
	const FChunkData& GetChunk(const FChunkIndex& Index) const;
	FORCEINLINE const FChunkData& GetChunk(const FWorldGridPos& GridPos) const;
	
	FORCEINLINE static FWorldGridPos WorldToGrid(const FVector& WorldPosition);
	FORCEINLINE static FVector GridToWorld(const FWorldGridPos& GridPos);
	
	UPROPERTY(Transient)
	TMap<FIntPoint, FChunkData> Chunks;
	
	WorldGenerator* Generator = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<AWorldRenderer> Renderer = nullptr;
	
private:
	void CreateChunk(const FChunkIndex& Index, FWorldRenderBatcher& RendererLock);
};
