#include "Grid/Generator/BiomeGenerator.h"

#include "Data/Definitions/BiomeDef.h"
#include "Settings/GridParams.h"

constexpr float RngBiomeMove = 1024.f;

BiomeGenerator::BiomeGenerator(const int32 Seed) :
		BaseGenerator(Seed),
		TemperatureOffset(GetRandomOffset({Seed}, RngBiomeMove)),
		HumidityOffset(GetRandomOffset(GetLocalRng(GetTypeHash(TemperatureOffset)), RngBiomeMove))
{
}

int8 BiomeGenerator::SampleBiome(const FWorldGridPos& Pos) const
{
	const float Temp = GetTemperature(Pos);
	const float Humidity = GetHumidity(Pos);
	const auto& Registry = GridParams::Get().GetBiomeRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (!IsEligible(Temp, Humidity, Registry[i])) continue;
		return i;
	}
	return -1;
}

bool BiomeGenerator::IsEligible(const FWorldGridPos& Pos, const FBiomeDefinition& Biome) const
{
	return IsEligible(GetTemperature(Pos), GetHumidity(Pos), Biome);
}

bool BiomeGenerator::IsEligible(const float Temp, const float Humidity, const FBiomeDefinition& Biome)
{
	return Biome.Temperature.Contains(Temp) && Biome.Humidity.Contains(Humidity);
}

float BiomeGenerator::GetTemperature(const FWorldGridPos& Pos) const
{
	return GetNoiseDensity(Pos, GridParams::Get().GetTemperature(), TemperatureOffset);
}

float BiomeGenerator::GetHumidity(const FWorldGridPos& Pos) const
{
	return GetNoiseDensity(Pos, GridParams::Get().GetHumidity(), HumidityOffset);
}
