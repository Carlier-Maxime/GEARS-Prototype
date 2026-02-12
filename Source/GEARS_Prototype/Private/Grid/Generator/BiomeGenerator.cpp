#include "Grid/Generator/BiomeGenerator.h"

#include "Data/BiomeType.h"
#include "Definitions/GEARS_Macro.h"
#include "Settings/GridParams.h"

constexpr float RngBiomeMove = 1024.f;

BiomeGenerator::BiomeGenerator(const int32 Seed) :
		BaseGenerator(Seed),
		TemperatureOffset(GetRandomOffset({Seed}, RngBiomeMove)),
		HumidityOffset(GetRandomOffset(GetLocalRng(GetTypeHash(TemperatureOffset)), RngBiomeMove))
{
}

int8 BiomeGenerator::SampleBiome(const FGridPosition& Pos) const
{
	const float Temp = GetTemperature(Pos);
	const float Humidity = GetHumidity(Pos);
	const auto& Registry = GridParams::Get().GetBiomeRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (!ensureSoftPtr(Registry[i])) continue;
		const auto Biome = Registry[i].LoadSynchronous();
		if (!IsEligible(Temp, Humidity, *Biome)) continue;
		return i;
	}
	return -1;
}

bool BiomeGenerator::IsEligible(const FGridPosition& Pos, const UBiomeType& Biome) const
{
	return IsEligible(GetTemperature(Pos), GetHumidity(Pos), Biome);
}

bool BiomeGenerator::IsEligible(const float Temp, const float Humidity, const UBiomeType& Biome)
{
	return Biome.Temperature.Contains(Temp) && Biome.Humidity.Contains(Humidity);
}

float BiomeGenerator::GetTemperature(const FGridPosition& Pos) const
{
	return GetNoiseDensity(Pos, GridParams::Get().GetTemperature(), TemperatureOffset);
}

float BiomeGenerator::GetHumidity(const FGridPosition& Pos) const
{
	return GetNoiseDensity(Pos, GridParams::Get().GetHumidity(), HumidityOffset);
}
