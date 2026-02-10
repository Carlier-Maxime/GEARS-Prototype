#include "Grid/Generator/BaseGenerator.h"

#include "Grid/Generator/Context/NoiseContext.h"

constexpr float RngSeedMove = 1024.f;
constexpr float RngResourceMove = 256.f;

BaseGenerator::BaseGenerator(const int32 Seed) :
		Seed(Seed), SeedOffset(GetRandomOffset({Seed}, RngSeedMove))
{}

FVector2D BaseGenerator::GetRandomOffset(const FRandomStream& RngStream, const float Displacement)
{
	return {
		RngStream.FRandRange(-Displacement, Displacement),
		RngStream.FRandRange(-Displacement, Displacement)
	};
}

FVector2D BaseGenerator::GetOffset(const FSoftObjectPath& Path) const
{
	return GetOffset(GetTypeHash(Path));
}

FVector2D BaseGenerator::GetOffset(const uint32 Hash) const
{
	return GetOrComputeOffset(Hash, RngResourceMove);
}

FVector2D BaseGenerator::GetOrComputeOffset(const uint32 Hash, const float Displacement) const
{
	{
		FReadScopeLock ReadLock(CacheLock);
		if (const auto Offset = CachedOffsets.Find(Hash)) return *Offset;
	}
	
	FWriteScopeLock WriteLock(CacheLock);
	if (const auto Offset = CachedOffsets.Find(Hash)) return *Offset;
	return CachedOffsets.Add(Hash, GetRandomOffset(GetLocalRng(Hash), Displacement));
}

float BaseGenerator::GetNoiseDensity(const FGridPosition& Pos, const FNoiseContext& Ctx, const FVector2D& Offset) const
{
	float Total = 0.0f;
	float Amplitude = 1.0f;
	float MaxValue = 0.0f;
	float CurrentFreq = Ctx.Frequency;
	
	const float SampleX = Pos.X + SeedOffset.X + Offset.X;
	const float SampleY = Pos.Y + SeedOffset.Y + Offset.Y;

	for (int32 i = 0; i < Ctx.Octaves; ++i)
	{
		const float NoiseVal = FMath::PerlinNoise2D(FVector2D(SampleX * CurrentFreq, SampleY * CurrentFreq));
        
		Total += NoiseVal * Amplitude;
		MaxValue += Amplitude;
        
		Amplitude *= Ctx.Persistence;
		CurrentFreq *= Ctx.Lacunarity;
	}
	
	return (Total / MaxValue + 1.0f) * 0.5f;
}

FRandomStream BaseGenerator::GetLocalRng(const FGridPosition& Pos) const
{
	return GetLocalRng(GetTypeHash(Pos));
}

FRandomStream BaseGenerator::GetLocalRng(const uint32 Hash) const
{
	return {static_cast<int32>(HashCombineFast(Seed, Hash))};
}
