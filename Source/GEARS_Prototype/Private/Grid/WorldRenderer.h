// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WorldRenderer.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class GEARS_PROTOTYPE_API AWorldRenderer : public AActor
{
	GENERATED_BODY()

public:
	AWorldRenderer();
	void UpdateResourcesInstances(const TArray<TArray<FTransform>>& ResourcesInstances);
	void AddPlane(const FIntPoint& ChunkIndex);
	void RemoveCheckedPlane(const FIntPoint& ChunkIndex);

protected:
	virtual void BeginPlay() override;
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> FindOrAddHISM(int16 ResourceIndex);
	
private:
	UPROPERTY(Transient)
	TMap<int16, TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> ResourcesComponents;
	UPROPERTY(Transient)
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> PlaneHISM;
	UPROPERTY(Transient)
	TMap<FIntPoint, int32> PlanesInstances;
};
