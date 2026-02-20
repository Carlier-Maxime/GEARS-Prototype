#pragma once
#include "Data/Registry/BiomeRegistry.h"
#include "Data/Registry/ResourceRegistry.h"
#include "Grid/Generator/Context/NoiseContext.h"

class UResourceType;
class UBiomeType;

class GEARS_PROTOTYPE_API GridParams
{
public:
	static GridParams& Get()
	{
		static GridParams Instance;
		return Instance;
	}
	
	FORCEINLINE float GetCellSize() const { return CellSize; }
	FORCEINLINE float GetInvCellSize() const { return InvCellSize; }
	FORCEINLINE uint32 GetChunkSize() const { return ChunkSize; }
	FORCEINLINE uint32 GetChunkSizeSquared() const { return ChunkSizeSquared; }
	FORCEINLINE uint32 GetChunkShift() const { return ChunkShift; }
	FORCEINLINE uint32 GetChunkMask() const { return ChunkMask; }
	FORCEINLINE uint16 GetMapRadius() const { return MapRadius; }
	FORCEINLINE const FResourceRegistry& GetResourceRegistry() const {return ResourceRegistry.GetValue();}
	FORCEINLINE const FBiomeRegistry& GetBiomeRegistry() const {return BiomeRegistry.GetValue();}
	FORCEINLINE const FNoiseContext& GetTemperature() const { return Temperature; }
	FORCEINLINE const FNoiseContext& GetHumidity() const { return Humidity; }
	FORCEINLINE const TObjectPtr<UStaticMesh>& GetGridMesh() const { return GridMesh; }
	FORCEINLINE const TObjectPtr<UTexture2DArray>& GetBiomeAtlas() const { return BiomeAtlas; }
private:
	GridParams() = default;
	friend class UGridSettings;
	float CellSize;
	float InvCellSize;
	uint32 ChunkSize;
	uint32 ChunkSizeSquared;
	uint32 ChunkShift;
	uint32 ChunkMask;
	uint16 MapRadius;
	TOptional<FResourceRegistry> ResourceRegistry;
	TOptional<FBiomeRegistry> BiomeRegistry;
	FNoiseContext Temperature;
	FNoiseContext Humidity;
	TObjectPtr<UStaticMesh> GridMesh;
	TObjectPtr<UTexture2DArray> BiomeAtlas;
};
