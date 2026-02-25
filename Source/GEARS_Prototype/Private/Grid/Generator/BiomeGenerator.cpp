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

uint8 BiomeGenerator::SampleBiome(const float Temp, const float Humidity)
{
	const auto& Registry = GridParams::Get().GetBiomeRegistry();
	auto MinDist = std::numeric_limits<float>::max();
	auto BestIndex = Registry.INVALID_INDEX;
	for (auto i=0; i<Registry.Num(); ++i)
	{
		for (const auto& Point : Registry[i].ClimateInfluences)
		{
			if (const auto Dist = Point.ManhattanDistance(Temp, Humidity); Dist < MinDist)
			{
				MinDist = Dist;
				BestIndex = i;
			}
		}
	}
	return BestIndex;
}

bool BiomeGenerator::IsEligible(const float Temp, const float Humidity, const FBiomeDefinition& Biome)
{
	return GridParams::Get().GetBiomeRegistry()[SampleBiome(Temp, Humidity)].Tag == Biome.Tag;
}

float BiomeGenerator::GetTemperature(const FWorldGridPos& Pos) const
{
	return GetNoiseDensity(Pos, GridParams::Get().GetTemperature(), TemperatureOffset);
}

float BiomeGenerator::GetHumidity(const FWorldGridPos& Pos) const
{
	return GetNoiseDensity(Pos, GridParams::Get().GetHumidity(), HumidityOffset);
}
