#pragma once
#include "Grid/Types/GridPosition.h"

class UResourceType;

class BaseGenerator
{
public:
	BaseGenerator(int32 Seed);
	virtual ~BaseGenerator() = default;
	[[nodiscard]] int16 SampleResourceAtPosition(const FGridPosition& Pos);
	[[nodiscard]] bool ShouldSpawnResource(const FGridPosition& Pos, const UResourceType& Resource) const;
	[[nodiscard]] bool ShouldSpawnResource(const FGridPosition& Pos, const UResourceType& Resource, const FVector2D& ResourceOffset) const;
	
protected:
	[[nodiscard]] FVector2D GetRandomResourceOffset() const;
	[[nodiscard]] FVector2D GetRandomSeedOffset() const;
	[[nodiscard]] FVector2D GetRandomOffset(float Displacement) const;
	
	FRandomStream RngStream;
	FVector2D SeedOffset;
	TArray<FVector2D> ResourcesOffset;
};
