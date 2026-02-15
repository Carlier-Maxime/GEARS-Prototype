// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRenderer.h"

#include "AI/NavigationSystemBase.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Data/ResourceType.h"
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

void AWorldRenderer::PostActorCreated()
{
	Super::PostActorCreated();
	const auto ResourcesCount = GridParams::Get().GetResourceRegistry().Num();
	ResourcesComponents.SetNum(ResourcesCount);
	ResourcesInstancesByChunk.SetNum(ResourcesCount);
}

void AWorldRenderer::BeginPlay()
{
	Super::BeginPlay();
	FNavigationSystem::Build(*GetWorld());
}

void AWorldRenderer::AddResourcesInstances(const FChunkIndex& Index, const TArray<TArray<FTransform>>& ResourcesInstances)
{
	for (auto i=0; i<ResourcesInstances.Num(); ++i)
	{
		const auto HISM = FindOrAddHISM(i);
		auto Indices = HISM->AddInstances(ResourcesInstances[i], true);
		ResourcesInstancesByChunk[i].FindOrAdd(Index).Append(Indices);
	}
}

void AWorldRenderer::RemoveCheckedResourcesInstances(const FChunkIndex& Index)
{
	for (auto i=0; i<ResourcesInstancesByChunk.Num(); ++i)
	{
		auto& InstancesByChunk = ResourcesInstancesByChunk[i];
		auto Indices = InstancesByChunk.FindAndRemoveChecked(Index);
		ResourcesComponents[i]->RemoveInstances(Indices);
	}
}

TObjectPtr<UHierarchicalInstancedStaticMeshComponent> AWorldRenderer::FindOrAddHISM(int16 ResourceIndex)
{
	if (const auto HISM = ResourcesComponents[ResourceIndex]) return *HISM;
	const auto& Resource = GridParams::Get().GetResourceRegistry()[ResourceIndex];
	const auto NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, Resource->ResourceTag.GetTagName());
	NewHISM->SetupAttachment(GetRootComponent());
	NewHISM->SetStaticMesh(Resource->WorldMesh.LoadSynchronous());
	NewHISM->SetMobility(EComponentMobility::Static);
	NewHISM->bEnableDensityScaling = true;
	NewHISM->SetCullDistances(0, GridParams::Get().GetCellSize() * (GridParams::Get().GetChunkSize() << 7));
	NewHISM->RegisterComponent();
	return ResourcesComponents[ResourceIndex] = NewHISM;
}

void AWorldRenderer::AddPlane(const FChunkIndex& Index)
{
	auto Transform = FTransform(Index.GetCenter());
	const auto Scale = GridParams::Get().GetChunkSize() * GridParams::Get().GetCellSize() * 0.01;
	Transform.SetScale3D(FVector(Scale, Scale, 1));
	PlanesInstances.Add(
		Index,
		PlaneHISM->AddInstance(Transform)
	);
}

void AWorldRenderer::RemoveCheckedPlane(const FChunkIndex& Index)
{
	const auto Id = PlanesInstances.FindAndRemoveChecked(Index);
	PlaneHISM->RemoveInstance(Id);
}
