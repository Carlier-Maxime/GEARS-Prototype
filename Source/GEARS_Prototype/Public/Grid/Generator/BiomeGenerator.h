#pragma once
#include "BaseGenerator.h"

class UBiomeType;

class GEARS_PROTOTYPE_API BiomeGenerator : public BaseGenerator
{
public:
	BiomeGenerator(int32 Seed);
	
	int8 SampleBiome(const FGridPosition& Pos) const;
	bool IsEligible(const FGridPosition& Pos, const UBiomeType& Biome) const;
	static bool IsEligible(float Temp, float Humidity, const UBiomeType& Biome);
	float GetTemperature(const FGridPosition& Pos) const;
	float GetHumidity(const FGridPosition& Pos) const;
	
private:
	FVector2D TemperatureOffset;
	FVector2D HumidityOffset;
};
