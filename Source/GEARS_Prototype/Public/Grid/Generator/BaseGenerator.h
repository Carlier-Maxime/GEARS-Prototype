#pragma once
#include "Grid/Types/WorldGridPos.h"

struct FNoiseContext;

class GEARS_PROTOTYPE_API BaseGenerator
{
public:
	BaseGenerator(int32 Seed);
	virtual ~BaseGenerator() = default;
	[[nodiscard]] float GetNoiseDensity(const FWorldGridPos& Pos, const FNoiseContext& Ctx, const FVector2D& Offset) const;
	
protected:
	[[nodiscard]] static FVector2D GetRandomOffset(const FRandomStream& RngStream, float Displacement);
	[[nodiscard]] FVector2D GetOrComputeOffset(uint32 Hash, float Displacement) const;
	[[nodiscard]] FVector2D GetOffset(uint32 Hash) const;
	[[nodiscard]] FVector2D GetOffset(const FSoftObjectPath& Path) const;
	template<typename T>
	[[nodiscard]] FVector2D GetOffset(const TSoftObjectPtr<T>& Ptr) const
	{
		return GetOffset(Ptr.ToSoftObjectPath());
	}
	
	[[nodiscard]] FRandomStream GetLocalRng(const FWorldGridPos& Pos) const;
	[[nodiscard]] FRandomStream GetLocalRng(uint32 Hash) const;
	
private:
	const int32 Seed;
	const FVector2D SeedOffset;
	
	mutable TMap<uint32, FVector2D> CachedOffsets;
	mutable FRWLock CacheLock;
};
