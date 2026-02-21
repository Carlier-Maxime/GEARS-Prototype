#pragma once
#include "Settings/GridParams.h"

namespace GridMath
{
	FORCEINLINE int32 Snap(double X)
	{
		return FMath::FloorToInt(X * GridParams::Get().GetInvCellSize());
	}
	
	FORCEINLINE double GridToWorld(int32 X)
	{
		return (X + 0.5f) * GridParams::Get().GetCellSize();
	}
	
	FORCEINLINE int32 GridToChunkIndex(int32 X)
	{
		return X >> GridParams::Get().GetChunkShift();
	}
	
	FORCEINLINE int32 GridToInChunk(int32 X)
	{
		return X & static_cast<int32>(GridParams::Get().GetChunkMask());
	}
	
	FORCEINLINE int32 ChunkToGrid(int32 X)
	{
		return X << GridParams::Get().GetChunkShift();
	}
	
	FORCEINLINE double ChunkToCenter(int32 X)
	{
		const auto& Params = GridParams::Get();
		return (ChunkToGrid(X) + static_cast<int32>(Params.GetChunkSize() >> 1)) * Params.GetCellSize();
	}
	
	FORCEINLINE int16 FlattenChunk(int32 X, int32 Y)
	{
		return X + Y * GridParams::Get().GetChunkSize();
	}
	
	FORCEINLINE uint16 GetChunkMapDiameter()
	{
		return (GridParams::Get().GetMapRadius()<<1) + 1;
	}
	
	FORCEINLINE uint32 GetChunkCount()
	{
		return GetChunkMapDiameter() * GetChunkMapDiameter();
	}
	
	FORCEINLINE uint32 GetBiomeChunkMapRadius()
	{
		const auto& Params = GridParams::Get();
		const uint32 Total = Params.GetMapRadius();
		const uint32 Shift = Params.GetBiomeChunkShift();
		const uint32 Divisor = 1 << Shift;
		return (Total + Divisor - 1) >> Shift;
	}
	
	FORCEINLINE uint32 GetBiomeChunkMapDiameter()
	{
		const uint32 Total = GetChunkMapDiameter();
		const uint32 Shift = GridParams::Get().GetBiomeChunkShift();
		const uint32 Divisor = 1 << Shift;
		return (Total + Divisor - 1) >> Shift;
	}
	
	FORCEINLINE uint32 GetBiomeChunkCount()
	{
		return GetBiomeChunkMapDiameter() * GetBiomeChunkMapDiameter();
	}
	
	FORCEINLINE int16 FlattenBiomeChunk(int32 X, int32 Y)
	{
		return X + Y * GridParams::Get().GetBiomeChunkFactor();
	}
	
	FORCEINLINE int32 ChunkToBiomeChunkIndex(int32 X)
	{
		return X >> GridParams::Get().GetBiomeChunkShift();
	}
	
	FORCEINLINE int32 ChunkToInBiomeChunk(int32 X)
	{
		return X & static_cast<int32>(GridParams::Get().GetBiomeChunkMask());
	}
	
	FORCEINLINE int32 BiomeChunkToChunk(int32 X)
	{
		return X << GridParams::Get().GetBiomeChunkShift();
	}
}
