#pragma once
#include "BaseGenerator.h"

struct FBiomeDefinition;

class GEARS_PROTOTYPE_API BiomeGenerator : public BaseGenerator
{
public:
	BiomeGenerator(int32 Seed);
	
	uint8 SampleBiome(const FWorldGridPos& Pos) const;
	bool IsEligible(const FWorldGridPos& Pos, const FBiomeDefinition& Biome) const;
	static bool IsEligible(float Temp, float Humidity, const FBiomeDefinition& Biome);
	float GetTemperature(const FWorldGridPos& Pos) const;
	float GetHumidity(const FWorldGridPos& Pos) const;
	
private:
	FVector2D TemperatureOffset;
	FVector2D HumidityOffset;
};
