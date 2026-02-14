#pragma once
#include "BaseGenerator.h"

class UBiomeType;

class GEARS_PROTOTYPE_API BiomeGenerator : public BaseGenerator
{
public:
	BiomeGenerator(int32 Seed);
	
	int8 SampleBiome(const FWorldGridPos& Pos) const;
	bool IsEligible(const FWorldGridPos& Pos, const UBiomeType& Biome) const;
	static bool IsEligible(float Temp, float Humidity, const UBiomeType& Biome);
	float GetTemperature(const FWorldGridPos& Pos) const;
	float GetHumidity(const FWorldGridPos& Pos) const;
	
private:
	FVector2D TemperatureOffset;
	FVector2D HumidityOffset;
};
