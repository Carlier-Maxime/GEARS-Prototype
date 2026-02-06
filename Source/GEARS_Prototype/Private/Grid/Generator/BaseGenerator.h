#pragma once
#include "Grid/Types/GridPosition.h"

struct FSamplingContext;

class BaseGenerator
{
public:
	BaseGenerator(int32 Seed);
	BaseGenerator(int32 Seed, bool bGenResourceOffset);
	virtual ~BaseGenerator() = default;
	[[nodiscard]] int16 SampleResourceAtPosition(const FGridPosition& Pos) const;
	[[nodiscard]] bool ShouldSpawnResource(const FGridPosition& Pos, const FSamplingContext& Ctx) const;
	
protected:
	[[nodiscard]] FVector2D GetRandomResourceOffset() const;
	[[nodiscard]] FVector2D GetRandomSeedOffset() const;
	[[nodiscard]] FVector2D GetRandomOffset(float Displacement) const;
	
	FRandomStream RngStream;
	FVector2D SeedOffset;
};
