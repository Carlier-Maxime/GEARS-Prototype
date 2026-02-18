// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRenderer.h"

#include "AI/NavigationSystemBase.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
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
	PlaneHISM->SetNumCustomDataFloats(1);
	PlaneHISM->SetVisibleInRayTracing(false);
	PlaneHISM->SetAffectDynamicIndirectLighting(false);
	
	const auto Scale = GridParams::Get().GetChunkSize() * GridParams::Get().GetCellSize() * 0.01;
	PlaneScale = FVector(Scale, Scale, 1);
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

TObjectPtr<UHierarchicalInstancedStaticMeshComponent> AWorldRenderer::FindOrAddHISM(int16 ResourceIndex)
{
	if (const auto& HISM = ResourcesComponents[ResourceIndex]) return HISM;
	const auto& Resource = GridParams::Get().GetResourceRegistry()[ResourceIndex];
	const auto NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, Resource.Tag.GetTagName());
	NewHISM->SetupAttachment(GetRootComponent());
	NewHISM->SetStaticMesh(Resource.WorldMesh.LoadSynchronous());
	NewHISM->SetMobility(EComponentMobility::Static);
	NewHISM->bEnableDensityScaling = true;
	NewHISM->SetCullDistances(0, GridParams::Get().GetCellSize() * (GridParams::Get().GetChunkSize() << 7));
	NewHISM->RegisterComponent();
	return ResourcesComponents[ResourceIndex] = NewHISM;
}

FWorldRenderScopedLock AWorldRenderer::Lock()
{
	return {*this};
}
