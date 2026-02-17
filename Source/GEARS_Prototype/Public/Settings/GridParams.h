#pragma once
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
	
	float GetCellSize() const { return CellSize; }
	float GetInvCellSize() const { return InvCellSize; }
	uint32 GetChunkSize() const { return ChunkSize; }
	uint32 GetChunkSizeSquared() const { return ChunkSizeSquared; }
	uint32 GetChunkShift() const { return ChunkShift; }
	uint32 GetChunkMask() const { return ChunkMask; }
	const TArray<TObjectPtr<UResourceType>>& GetResourceRegistry() const {return ResourceRegistry;}
	const TArray<TObjectPtr<UBiomeType>>& GetBiomeRegistry() const {return BiomeRegistry;}
	const FNoiseContext& GetTemperature() const { return Temperature; }
	const FNoiseContext& GetHumidity() const { return Humidity; }
	const TObjectPtr<UStaticMesh>& GetGridMesh() const { return GridMesh; }
	const TObjectPtr<UTexture2DArray>& GetBiomeAtlas() const { return BiomeAtlas; }
private:
	GridParams() = default;
	friend class UGridSettings;
	float CellSize;
	float InvCellSize;
	uint32 ChunkSize;
	uint32 ChunkSizeSquared;
	uint32 ChunkShift;
	uint32 ChunkMask;
	TArray<TObjectPtr<UResourceType>> ResourceRegistry;
	TArray<TObjectPtr<UBiomeType>> BiomeRegistry;
	FNoiseContext Temperature;
	FNoiseContext Humidity;
	TObjectPtr<UStaticMesh> GridMesh;
	TObjectPtr<UTexture2DArray> BiomeAtlas;
};
