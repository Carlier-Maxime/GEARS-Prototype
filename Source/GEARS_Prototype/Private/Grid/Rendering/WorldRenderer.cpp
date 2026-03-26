// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRenderer.h"

#include "AI/NavigationSystemBase.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameplayTags/GEARS_GameplayTags.h"
#include "Grid/GridSubsystem.h"
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
	PlaneHISM->bAutoRebuildTreeOnInstanceChanges = false;
	
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
	NewHISM->bAutoRebuildTreeOnInstanceChanges = false;
	NewHISM->SetCullDistances(0, GridParams::Get().GetCellSize() * (GridParams::Get().GetChunkSize() << 7));
	NewHISM->RegisterComponent();
	return ResourcesComponents[ResourceIndex] = NewHISM;
}

TArray<int32>& AWorldRenderer::FindOrAddResourceInstances(const int16 ResourceIndex, const FChunkIndex& Chunk)
{
	auto& InstancesByChunk = ResourcesInstancesByChunk[ResourceIndex];
	auto Instances = InstancesByChunk.Find(Chunk);
	if (Instances) return *Instances;
	auto& NewInstances = InstancesByChunk.Emplace(Chunk);
	NewInstances.Init(INDEX_NONE, GridParams::Get().GetChunkSizeSquared());
	return NewInstances;
}

FWorldRenderBatcher AWorldRenderer::Batcher()
{
	return {*this};
}

FGameplayTag AWorldRenderer::GetTypeTag(const FHitResult& Hit) const
{
	if (!Hit.Component.IsValid()) return FGameplayTag::EmptyTag;
	auto* HISM = Cast<UHierarchicalInstancedStaticMeshComponent>(Hit.Component.Get());
	if (HISM == PlaneHISM) return GetBiomeTag(Hit);
	auto Index = ResourcesComponents.Find(HISM);
	if (Index != INDEX_NONE) return GridParams::Get().GetResourceRegistry()[Index].Tag;
	return FGameplayTag::EmptyTag;
}

FGameplayTag AWorldRenderer::GetBiomeTag(const FWorldGridPos& Pos) const
{
	auto* Grid = GetWorld()->GetSubsystem<UGridSubsystem>();
	if (!Grid) return FGameplayTag::EmptyTag;
	return Grid->GetBiome(Pos).Tag;
}
