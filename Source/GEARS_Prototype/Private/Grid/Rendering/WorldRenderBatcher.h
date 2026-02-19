#pragma once
#include "BiomeAtlasScopedLock.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Grid/Types.h"

class AWorldRenderer;

struct FChunkRenderData
{
	FChunkIndex Index;
	TArray<TArray<FTransform>> Resources;
	TArray<int8> BiomeMap;
};

class FWorldRenderBatcher
{
public:
	FWorldRenderBatcher(AWorldRenderer& InRenderer);
	~FWorldRenderBatcher();
	
	void ReserveTasks(int32 Count);
	void AddChunkTask(FChunkRenderData&& ChunkData);
	void Apply();

private:
	void ApplyResourcesInstances(const FChunkIndex& Index, const TArray<TArray<FTransform>>& ResourcesInstances);
	void ApplyPlane(const FChunkIndex& Index, const TArray<int8>& BiomeMap);	
	
	AWorldRenderer& Renderer;
	FBiomeAtlasScopedLock BiomeTextureMapper;
	FCriticalSection DataGuard;
	TArray<FChunkRenderData> PendingAdds;
	TSet<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> UpdatedHISMs;
};
