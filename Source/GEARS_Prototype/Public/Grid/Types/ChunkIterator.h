#pragma once

#include "InChunkPos.h"
#include "Settings/GridParams.h"

struct FChunkIterator
{
	FORCEINLINE FInChunkPos operator*() const {return Pos;}
	
	FORCEINLINE FChunkIterator& operator++()
	{
		Pos.X++;
		if (Pos.X >= static_cast<int32>(GridParams::Get().GetChunkSize())) { Pos.X = 0; Pos.Y++; }
		return *this;
	}
	
	FORCEINLINE bool operator!=(const FChunkIterator& Other) const{
		return Pos.Y != Other.Pos.Y || Pos.X != Other.Pos.X;
	}
	
	FORCEINLINE static auto Begin() {return FChunkIterator(FInChunkPos::Start);}
	FORCEINLINE static auto End() {return FChunkIterator(FInChunkPos::PostEnd());}

private:
	FORCEINLINE explicit FChunkIterator(const FInChunkPos& InPos) : Pos(InPos) {}
	FInChunkPos Pos;
};
