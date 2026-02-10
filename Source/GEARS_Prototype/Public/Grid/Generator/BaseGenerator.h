#pragma once
#include "Grid/Types/GridPosition.h"

struct FNoiseContext;
struct FSamplingContext;
struct FProcSpawnData;

class GEARS_PROTOTYPE_API BaseGenerator
{
public:
	BaseGenerator(int32 Seed);
	virtual ~BaseGenerator() = default;
	[[nodiscard]] FProcSpawnData SampleResource(const FGridPosition& Pos) const;
	[[nodiscard]] bool ShouldSpawnResource(const FGridPosition& Pos, const FSamplingContext& Ctx) const;
	[[nodiscard]] float GetNoiseDensity(const FGridPosition& Pos, const FNoiseContext& Ctx) const;
	
protected:
	[[nodiscard]] static FVector2D GetRandomOffset(const FRandomStream& RngStream, float Displacement);
	[[nodiscard]] FVector2D GetOrComputeOffset(uint32 Hash, float Displacement) const;
	[[nodiscard]] FVector2D GetResourceOffset(uint32 Hash) const;
	[[nodiscard]] FVector2D GetResourceOffset(const FSoftObjectPath& Path) const;
	template<typename T>
	[[nodiscard]] FVector2D GetResourceOffset(const TSoftObjectPtr<T>& Ptr) const
	{
		return GetResourceOffset(Ptr.ToSoftObjectPath());
	}
	
	[[nodiscard]] int16 DetermineResourceType(const FGridPosition& Pos) const;
	[[nodiscard]] FTransform GetVariationTransform(const FGridPosition& Pos, int16 ResourceTypeIndex) const;
	[[nodiscard]] FRandomStream GetLocalRng(const FGridPosition& Pos) const;
	[[nodiscard]] FRandomStream GetLocalRng(uint32 Hash) const;
	
private:
	const int32 Seed;
	const FVector2D SeedOffset;
	
	mutable TMap<uint32, FVector2D> CachedOffsets;
	mutable FRWLock CacheLock;
};
