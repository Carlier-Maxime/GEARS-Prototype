#pragma once

#include "ProcSpawnData.h"
#include "Grid/Generator/BaseGenerator.h"

struct FDistributionRule;
struct FPlacementRule;

class GEARS_PROTOTYPE_API ResourceGenerator : public BaseGenerator
{
public:
	ResourceGenerator(int32 Seed);
	
	[[nodiscard]] FProcSpawnData Sample(const FWorldGridPos& Pos, const TArray<FResourceRule>& Rules) const;
	[[nodiscard]] bool ShouldSpawn(const FWorldGridPos& Pos, const FDistributionRule& Rule, const FVector2D& Offset) const;
	
protected:
	[[nodiscard]] int16 DetermineType(const FWorldGridPos& Pos, const TArray<FResourceRule>& Rules) const;
	[[nodiscard]] FTransform GetVariationTransform(const FWorldGridPos& Pos, int16 ResourceTypeIndex) const;
	[[nodiscard]] FTransform GetVariationTransform(const FWorldGridPos& Pos, const FPlacementRule& Rule) const;
};
