#pragma once
#include "BiomeIndexMapScopedLock.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Grid/Types.h"

class AWorldRenderer;

struct FChunkRenderData
{
	FChunkIndex Index;
	TArray<TArray<FTransform>> Resources;
	TArray<uint8> BiomeMap;
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
	void ApplyPlane(const FChunkIndex& Index, const TArray<uint8>& BiomeMap);	
	
	AWorldRenderer& Renderer;
	FBiomeIndexMapScopedLock BiomeTextureMapper;
	FCriticalSection DataGuard;
	TArray<FChunkRenderData> PendingAdds;
	TSet<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> UpdatedHISMs;
};
