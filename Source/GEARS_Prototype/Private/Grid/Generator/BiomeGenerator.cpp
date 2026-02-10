#include "Grid/Generator/BiomeGenerator.h"

#include "Data/BiomeType.h"
#include "Definitions/GEARS_Macro.h"
#include "Settings/GridParams.h"

constexpr float RngBiomeMove = 1024.f;

BiomeGenerator::BiomeGenerator(const int32 Seed) :
		BaseGenerator(Seed),
		TemperatureOffset(GetRandomOffset({Seed}, RngBiomeMove)),
		HumidityOffset(GetRandomOffset({Seed}, RngBiomeMove))
{
}

int8 BiomeGenerator::SampleBiome(const FGridPosition& Pos) const
{
	const auto& Params = GridParams::Get();
	const float Temp = GetNoiseDensity(Pos, Params.GetTemperature(), TemperatureOffset);
	const float Humidity = GetNoiseDensity(Pos, Params.GetHumidity(), HumidityOffset);
	const auto& Registry = Params.GetBiomeRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (!ensureSoftPtr(Registry[i])) continue;
		const auto Biome = Registry[i].LoadSynchronous();
		if (!Biome->Temperature.Contains(Temp) || !Biome->Humidity.Contains(Humidity)) continue;
		return i;
	}
	return -1;
}
