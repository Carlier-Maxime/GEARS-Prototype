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
}
