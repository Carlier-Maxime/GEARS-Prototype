#pragma once

class GridParams
{
public:
	static float GetCellSize() { return CellSize; }
	static float GetInvCellSize() { return InvCellSize; }
	static uint32 GetChunkSize() { return ChunkSize; }
	static uint32 GetChunkSizeSquared() { return ChunkSizeSquared; }
private:
	friend class UGridSettings;
	inline static float CellSize;
	inline static float InvCellSize;
	inline static uint32 ChunkSize;
	inline static uint32 ChunkSizeSquared;
};
