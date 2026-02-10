#pragma once
#include "BaseGenerator.h"

struct FBiomeType;

class GEARS_PROTOTYPE_API BiomeGenerator : public BaseGenerator
{
public:
	BiomeGenerator(int32 Seed);
	
	int8 SampleBiome(const FGridPosition& Pos) const;
	
private:
	FVector2D TemperatureOffset;
	FVector2D HumidityOffset;
};
