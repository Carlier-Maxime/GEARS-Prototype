#include "WorldRenderScopedLock.h"

#include "WorldRenderer.h"

FWorldRenderScopedLock::FWorldRenderScopedLock(AWorldRenderer& InRenderer) : 
		Renderer(InRenderer), BiomeTextureMapper(Renderer.BiomeTextureMapper.Lock())
{}

FWorldRenderScopedLock::~FWorldRenderScopedLock()
{
	for (auto& HISM : UpdatedHISMs)
	{
		HISM->MarkRenderStateDirty();
		HISM->MarkRenderInstancesDirty();
		HISM->BuildTreeIfOutdated(true, false);
	}
}

void FWorldRenderScopedLock::AddResourcesInstances(const FChunkIndex& Index,
	const TArray<TArray<FTransform>>& ResourcesInstances)
{
	for (auto i=0; i<ResourcesInstances.Num(); ++i)
	{
		const auto HISM = Renderer.FindOrAddHISM(i);
		if (ResourcesInstances[i].IsEmpty()) continue;
		auto Indices = HISM->AddInstances(ResourcesInstances[i], true);
		Renderer.ResourcesInstancesByChunk[i].FindOrAdd(Index).Append(Indices);
		UpdatedHISMs.Add(HISM);
	}
}

void FWorldRenderScopedLock::RemoveCheckedResourcesInstances(const FChunkIndex& Index)
{
	for (auto i=0; i<Renderer.ResourcesInstancesByChunk.Num(); ++i)
	{
		auto& InstancesByChunk = Renderer.ResourcesInstancesByChunk[i];
		auto Indices = InstancesByChunk.FindAndRemoveChecked(Index);
		auto HISM = Renderer.ResourcesComponents[i];
		HISM->RemoveInstances(Indices);
		UpdatedHISMs.Add(HISM);
	}
}

void FWorldRenderScopedLock::AddPlane(const FChunkIndex& Index, const TArray<int8>& BiomeMap)
{
	auto Transform = FTransform(Index.GetCenter());
	Transform.SetScale3D(Renderer.PlaneScale);
	const auto Instance = Renderer.PlanesInstances.Add(
		Index,
		Renderer.PlaneHISM->AddInstance(Transform)
	);
	BiomeTextureMapper.UpdateSliceFromBiomeTypes(Instance, BiomeMap);
	Renderer.PlaneHISM->SetCustomDataValue(Instance, 0, static_cast<float>(Instance));
	UpdatedHISMs.Add(Renderer.PlaneHISM);
}

void FWorldRenderScopedLock::RemoveCheckedPlane(const FChunkIndex& Index)
{
	const auto Id = Renderer.PlanesInstances.FindAndRemoveChecked(Index);
	Renderer.PlaneHISM->RemoveInstance(Id);
	UpdatedHISMs.Add(Renderer.PlaneHISM);
}
