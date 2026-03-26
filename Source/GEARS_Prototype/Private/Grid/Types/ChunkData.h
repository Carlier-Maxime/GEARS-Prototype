#pragma once
#include "Grid/Types.h"

#include "ChunkData.generated.h"

USTRUCT()
struct FChunkData
{
	GENERATED_BODY()
	FChunkData();
	FORCEINLINE int16 GetResourceIndex(const FInChunkPos& Pos) const;
	FORCEINLINE int16 GetResourceIndex(const FWorldGridPos& GridPos) const;
	const FResourceState* GetResourceState(const FInChunkPos& Pos) const;
	FORCEINLINE const FResourceState* GetResourceState(const FWorldGridPos& GridPos) const;
	FResourceState* GetMutableResourceState(const FInChunkPos& Pos);
	FORCEINLINE FResourceState* GetMutableResourceState(const FWorldGridPos& GridPos);
	void ResetResourceState(const FInChunkPos& Pos);
	FORCEINLINE void ResetResourceState(const FWorldGridPos& GridPos);
	void SetResource(const FInChunkPos& Pos, int16 ResourceIndex);
	FORCEINLINE void SetResource(const FWorldGridPos& GridPos, int16 ResourceIndex);
	
	FORCEINLINE uint8 GetBiomeIndex(const FWorldGridPos& GridPos) const;
	FORCEINLINE uint8 GetBiomeIndex(const FInChunkPos& Pos) const;
	FORCEINLINE const FBiomeDefinition& GetBiome(const FWorldGridPos& GridPos) const;
	FORCEINLINE const FBiomeDefinition& GetBiome(const FInChunkPos& Pos) const;
	
	void SetBiome(const FInChunkPos& Pos, uint8 Biome);
	FORCEINLINE void SetBiome(const FWorldGridPos& GridPos, uint8 Biome);
	FORCEINLINE const TArray<uint8>& GetBiomeMap() const {return BiomeMap;}
private:
	TArray<int16> ResourceMap;
	TArray<uint8> BiomeMap;
	TArray<bool> IsDynamic;
	TMap<int16, FResourceState> DynamicStates;
};

void FChunkData::SetResource(const FWorldGridPos& GridPos, const int16 ResourceIndex)
{
	return SetResource(GridPos.ToInChunkPos(), ResourceIndex);
}

inline void FChunkData::SetBiome(const FWorldGridPos& GridPos, const uint8 Biome)
{
	return SetBiome(GridPos.ToInChunkPos(), Biome);
}

inline uint8 FChunkData::GetBiomeIndex(const FWorldGridPos& GridPos) const
{
	return GetBiomeIndex(GridPos.ToInChunkPos());
}

inline uint8 FChunkData::GetBiomeIndex(const FInChunkPos& Pos) const
{
	return BiomeMap[Pos.Flatten()];
}

inline const FBiomeDefinition& FChunkData::GetBiome(const FWorldGridPos& GridPos) const
{
	return GetBiome(GridPos.ToInChunkPos());
}

inline const FBiomeDefinition& FChunkData::GetBiome(const FInChunkPos& Pos) const
{
	return GridParams::Get().GetBiomeRegistry()[GetBiomeIndex(Pos)];
}

inline int16 FChunkData::GetResourceIndex(const FInChunkPos& Pos) const
{
	return ResourceMap[Pos.Flatten()];
}

inline int16 FChunkData::GetResourceIndex(const FWorldGridPos& GridPos) const
{
	return GetResourceIndex(GridPos.ToInChunkPos());
}

inline const FResourceState* FChunkData::GetResourceState(const FWorldGridPos& GridPos) const
{
	return GetResourceState(GridPos.ToInChunkPos());
}

inline FResourceState* FChunkData::GetMutableResourceState(const FWorldGridPos& GridPos)
{
	return GetMutableResourceState(GridPos.ToInChunkPos());
}

inline void FChunkData::ResetResourceState(const FWorldGridPos& GridPos)
{
	ResetResourceState(GridPos.ToInChunkPos());
}