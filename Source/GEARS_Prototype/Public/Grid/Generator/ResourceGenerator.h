#pragma once

#include "ProcSpawnData.h"
#include "Grid/Generator/BaseGenerator.h"

class GEARS_PROTOTYPE_API ResourceGenerator : public BaseGenerator
{
public:
	ResourceGenerator(int32 Seed);
	
	[[nodiscard]] FProcSpawnData Sample(const FGridPosition& Pos) const;
	[[nodiscard]] bool ShouldSpawn(const FGridPosition& Pos, const FSamplingContext& Ctx, const FVector2D& Offset) const;
	
protected:
	[[nodiscard]] int16 DetermineType(const FGridPosition& Pos) const;
	[[nodiscard]] FTransform GetVariationTransform(const FGridPosition& Pos, int16 ResourceTypeIndex) const;
};
