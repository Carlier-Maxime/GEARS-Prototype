#pragma once
#include "BiomeAtlasScopedLock.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

struct FChunkIndex;
class AWorldRenderer;

class FWorldRenderScopedLock
{
public:
	FWorldRenderScopedLock(AWorldRenderer& InRenderer);
	~FWorldRenderScopedLock();
	
	void AddResourcesInstances(const FChunkIndex& Index, const TArray<TArray<FTransform>>& ResourcesInstances);
	void RemoveCheckedResourcesInstances(const FChunkIndex& Index);
	void AddPlane(const FChunkIndex& Index, const TArray<int8>& BiomeMap);
	void RemoveCheckedPlane(const FChunkIndex& Index);
private:
	AWorldRenderer& Renderer;
	FBiomeAtlasScopedLock BiomeTextureMapper;
	TSet<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> UpdatedHISMs;
};
