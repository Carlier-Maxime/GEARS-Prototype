#include "WorldRenderBatcher.h"

#include "WorldRenderer.h"

FWorldRenderBatcher::FWorldRenderBatcher(AWorldRenderer& InRenderer) : 
		Renderer(InRenderer), BiomeTextureMapper(Renderer.BiomeTextureMapper.Lock())
{
	check(IsInGameThread());
}

FWorldRenderBatcher::~FWorldRenderBatcher()
{
	Apply();
}

void FWorldRenderBatcher::Apply()
{
	check(IsInGameThread());
	for (const auto& [Index, Resources, BiomeMap] : PendingAdds)
	{
		ApplyPlane(Index, BiomeMap);
		ApplyResourcesInstances(Index, Resources);
	}
	PendingAdds.Reset();
	for (auto& HISM : UpdatedHISMs)
	{
		HISM->MarkRenderStateDirty();
		HISM->MarkRenderInstancesDirty();
		HISM->BuildTreeIfOutdated(true, false);
	}
	UpdatedHISMs.Reset();
}

void FWorldRenderBatcher::ReserveTasks(int32 Count)
{
	PendingAdds.Reserve(Count);
}

void FWorldRenderBatcher::AddChunkTask(FChunkRenderData&& ChunkData)
{
	FScopeLock Lock(&DataGuard);
	PendingAdds.Add(std::move(ChunkData));
}

void FWorldRenderBatcher::ApplyResourcesInstances(const FChunkIndex& Index, const TArray<TArray<FTransform>>& ResourcesInstances)
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

void FWorldRenderBatcher::ApplyPlane(const FChunkIndex& Index, const TArray<int8>& BiomeMap)
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
