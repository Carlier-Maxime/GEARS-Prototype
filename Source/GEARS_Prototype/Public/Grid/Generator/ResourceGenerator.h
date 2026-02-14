#pragma once

#include "ProcSpawnData.h"
#include "Grid/Generator/BaseGenerator.h"

class GEARS_PROTOTYPE_API ResourceGenerator : public BaseGenerator
{
public:
	ResourceGenerator(int32 Seed);
	
	[[nodiscard]] FProcSpawnData Sample(const FWorldGridPos& Pos) const;
	[[nodiscard]] bool ShouldSpawn(const FWorldGridPos& Pos, const FSamplingContext& Ctx, const FVector2D& Offset) const;
	
protected:
	[[nodiscard]] int16 DetermineType(const FWorldGridPos& Pos) const;
	[[nodiscard]] FTransform GetVariationTransform(const FWorldGridPos& Pos, int16 ResourceTypeIndex) const;
};
