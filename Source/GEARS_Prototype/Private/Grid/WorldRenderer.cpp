// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRenderer.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Data/ResourceType.h"
#include "Settings/GridParams.h"


AWorldRenderer::AWorldRenderer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Static);
	SetRootComponent(Root);
}

void AWorldRenderer::BeginPlay()
{
	Super::BeginPlay();
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
