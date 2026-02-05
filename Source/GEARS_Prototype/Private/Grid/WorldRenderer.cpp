// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldRenderer.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Definitions/GEARS_Macro.h"
#include "Settings/GridParams.h"


AWorldRenderer::AWorldRenderer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWorldRenderer::BeginPlay()
{
	Super::BeginPlay();
	auto Registry = GridParams::Get().GetResourceRegistry();
	ResourcesComponents.Reserve(Registry.Num());
	for (auto SoftResource: Registry)
	{
		if (!ensureSoftPtr(SoftResource)) continue;
		const auto Resource = SoftResource.LoadSynchronous();
		auto NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
		NewHISM->RegisterComponent();
		NewHISM->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		NewHISM->SetStaticMesh(Resource->WorldMesh);
		NewHISM->SetMobility(EComponentMobility::Static);
		NewHISM->bEnableDensityScaling = true;
		NewHISM->SetCullDistances(0, GridParams::Get().GetCellSize() * (GridParams::Get().GetChunkSize() << 7));
		ResourcesComponents.Add(NewHISM);
	}
}
