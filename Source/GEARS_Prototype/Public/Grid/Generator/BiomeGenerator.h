#pragma once
#include "BaseGenerator.h"

struct FBiomeDefinition;

class GEARS_PROTOTYPE_API BiomeGenerator : public BaseGenerator
{
public:
	BiomeGenerator(int32 Seed);
	
	static uint8 SampleBiome(float Temp, float Humidity);
	FORCEINLINE uint8 SampleBiome(const FWorldGridPos& Pos) const
	{
		return SampleBiome(GetTemperature(Pos), GetHumidity(Pos));
	}
	
	FORCEINLINE bool IsEligible(const FWorldGridPos& Pos, const FBiomeDefinition& Biome) const
	{
		return IsEligible(GetTemperature(Pos), GetHumidity(Pos), Biome);
	}
	static bool IsEligible(float Temp, float Humidity, const FBiomeDefinition& Biome);
	
	float GetTemperature(const FWorldGridPos& Pos) const;
	float GetHumidity(const FWorldGridPos& Pos) const;
	
private:
	FVector2D TemperatureOffset;
	FVector2D HumidityOffset;
};
