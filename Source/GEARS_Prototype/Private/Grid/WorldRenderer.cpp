// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRenderer.h"

#include "AI/NavigationSystemBase.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Data/ResourceType.h"
#include "Grid/Types/GridPosition.h"
#include "Settings/GridParams.h"


AWorldRenderer::AWorldRenderer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);
	
	PlaneHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("PlaneHISM"));
	PlaneHISM->SetMobility(EComponentMobility::Static);
	PlaneHISM->SetStaticMesh(GridParams::Get().GetGridMesh());
	PlaneHISM->SetupAttachment(Root);
	PlaneHISM->SetCullDistances(0, GridParams::Get().GetCellSize() * (GridParams::Get().GetChunkSize() << 10));
}

void AWorldRenderer::BeginPlay()
{
	Super::BeginPlay();
	FNavigationSystem::Build(*GetWorld());
}

void AWorldRenderer::UpdateResourcesInstances(const TArray<TArray<FTransform>>& ResourcesInstances)
{
	for (auto i=0; i<ResourcesInstances.Num(); ++i)
	{
		if (ResourcesInstances[i].Num() == 0)
		{
			if (const auto FoundHISM = ResourcesComponents.Find(i)) (*FoundHISM)->ClearInstances();
			continue;
		}
		const auto HISM = FindOrAddHISM(i);
		HISM->ClearInstances();
		HISM->AddInstances(ResourcesInstances[i], false);
	}
}

TObjectPtr<UHierarchicalInstancedStaticMeshComponent> AWorldRenderer::FindOrAddHISM(int16 ResourceIndex)
{
	if (const auto HISM = ResourcesComponents.Find(ResourceIndex)) return *HISM;
	const auto& Resource = GridParams::Get().GetResourceRegistry()[ResourceIndex];
	const auto NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, Resource->ResourceTag.GetTagName());
	NewHISM->SetupAttachment(GetRootComponent());
	NewHISM->SetStaticMesh(Resource->WorldMesh.LoadSynchronous());
	NewHISM->SetMobility(EComponentMobility::Static);
	NewHISM->bEnableDensityScaling = true;
	NewHISM->SetCullDistances(0, GridParams::Get().GetCellSize() * (GridParams::Get().GetChunkSize() << 7));
	NewHISM->RegisterComponent();
	return ResourcesComponents.Add(ResourceIndex, NewHISM);
}

void AWorldRenderer::AddPlane(const FIntPoint& ChunkIndex)
{
	auto Transform = FGridPosition::FromChunkIndex(ChunkIndex).ToTransform(); // TODO Center Plane
	const auto Scale = GridParams::Get().GetChunkSize() * GridParams::Get().GetCellSize() * 0.01;
	Transform.SetScale3D(FVector(Scale, Scale, 1));
	PlanesInstances.Add(
		ChunkIndex,
		PlaneHISM->AddInstance(Transform)
	);
}

void AWorldRenderer::RemoveCheckedPlane(const FIntPoint& ChunkIndex)
{
	const auto Id = PlanesInstances.FindAndRemoveChecked(ChunkIndex);
	PlaneHISM->RemoveInstance(Id);
}
