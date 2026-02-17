// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	void AddResourcesInstances(const FChunkIndex& Index, const TArray<TArray<FTransform>>& ResourcesInstances);
	void RemoveCheckedResourcesInstances(const FChunkIndex& Index);
	void AddPlane(const FChunkIndex& Index);
	void RemoveCheckedPlane(const FChunkIndex& Index);

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
};
