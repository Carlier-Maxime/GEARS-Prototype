// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomeTextureMapper.h"
#include "WorldRenderBatcher.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Grid/Types.h"
#include "WorldRenderer.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class GEARS_PROTOTYPE_API AWorldRenderer : public AActor
{
	GENERATED_BODY()

public:
	AWorldRenderer();
	virtual void PostActorCreated() override;
	FWorldRenderBatcher Batcher();

protected:
	virtual void BeginPlay() override;
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> FindOrAddHISM(int16 ResourceIndex);
	
private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> ResourcesComponents;
	TArray<TMap<FChunkIndex, TArray<int32>>> ResourcesInstancesByChunk;
	
	UPROPERTY(Transient)
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> PlaneHISM;
	TMap<FChunkIndex, int32> PlanesInstances;
	FVector PlaneScale;
	
	FBiomeTextureMapper BiomeTextureMapper;
	
	friend class FWorldRenderBatcher;
};
