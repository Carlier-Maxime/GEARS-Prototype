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
	[[nodiscard]] FProcSpawnData SampleResourceAtPosition(const FGridPosition& Pos) const;
	[[nodiscard]] bool ShouldSpawnResource(const FGridPosition& Pos, const FSamplingContext& Ctx) const;
	[[nodiscard]] float GetNoiseDensityAtPosition(const FGridPosition& Pos, const FNoiseContext& Ctx) const;
	
protected:
	[[nodiscard]] FVector2D GetRandomResourceOffset() const;
	[[nodiscard]] FVector2D GetRandomSeedOffset() const;
	[[nodiscard]] FVector2D GetRandomOffset(float Displacement) const;
	[[nodiscard]] int16 DetermineResourceType(const FGridPosition& Pos) const;
	[[nodiscard]] FTransform CalculateVariationTransform(const FGridPosition& Pos, int16 ResourceTypeIndex) const;
	[[nodiscard]] FRandomStream GetLocalRng(const FGridPosition& Pos) const;
	
	const int32 Seed;
	const FRandomStream RngStream;
	const FVector2D SeedOffset;
};
