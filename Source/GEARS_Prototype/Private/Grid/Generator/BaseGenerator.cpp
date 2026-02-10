#include "Grid/Generator/BaseGenerator.h"

#include "ProcSpawnData.h"
#include "Grid/Generator/Context/SamplingContext.h"
#include "Definitions/GEARS_Macro.h"
#include "Settings/GridParams.h"

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

FVector2D BaseGenerator::GetResourceOffset(const FSoftObjectPath& Path) const
{
	return GetResourceOffset(GetTypeHash(Path));
}

FVector2D BaseGenerator::GetResourceOffset(const uint32 Hash) const
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

FProcSpawnData BaseGenerator::SampleResource(const FGridPosition& Pos) const
{
	FProcSpawnData SpawnData;
	SpawnData.ResourceTypeIndex = DetermineResourceType(Pos);
	SpawnData.Transform = GetVariationTransform(Pos, SpawnData.ResourceTypeIndex);
	return std::move(SpawnData);
}

int16 BaseGenerator::DetermineResourceType(const FGridPosition& Pos) const
{
	const auto& Registry = GridParams::Get().GetResourceRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (!ensureSoftPtr(Registry[i])) continue;
		auto& Sampling = Registry[i].LoadSynchronous()->Sampling;
		Sampling.Noise.Offset = GetResourceOffset(Registry[i]);
		if (ShouldSpawnResource(Pos, Sampling)) return i;
	}
	return -1;
}

bool BaseGenerator::ShouldSpawnResource(const FGridPosition& Pos, const FSamplingContext& Ctx) const
{
	const auto NoiseDensity = GetNoiseDensity(Pos, Ctx.Noise);
	if (Ctx.ThresholdSmoothing == 0) return NoiseDensity >= Ctx.NoiseThreshold;
	const float SpawnChance = FMath::SmoothStep(
		Ctx.NoiseThreshold,
		Ctx.NoiseThreshold + Ctx.ThresholdSmoothing, 
		NoiseDensity
	);
	if (SpawnChance <= 0.f) return false;
	if (SpawnChance >= 1.f) return true;
	return GetLocalRng(Pos).FRand() < SpawnChance;
}

float BaseGenerator::GetNoiseDensity(const FGridPosition& Pos, const FNoiseContext& Ctx) const
{
	float Total = 0.0f;
	float Amplitude = 1.0f;
	float MaxValue = 0.0f;
	float CurrentFreq = Ctx.Frequency;
	
	const float SampleX = Pos.X + SeedOffset.X + Ctx.Offset.X;
	const float SampleY = Pos.Y + SeedOffset.Y + Ctx.Offset.Y;

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

FTransform BaseGenerator::GetVariationTransform(const FGridPosition& Pos, const int16 ResourceTypeIndex) const
{
	auto Transform = Pos.ToTransform();
	if (ResourceTypeIndex == -1) return std::move(Transform);
	const auto& Sampling = GridParams::Get().GetResourceRegistry()[ResourceTypeIndex].LoadSynchronous()->Sampling;
	const auto LocalRng = GetLocalRng(Pos);
	if (Sampling.JitterMaxOffset > 0)
	{
		const auto Loc = Transform.GetLocation();
		Transform.SetLocation({
			Loc.X + GridParams::Get().GetCellSize() * Sampling.JitterMaxOffset * LocalRng.FRandRange(-1, 1),
			Loc.Y + GridParams::Get().GetCellSize() * Sampling.JitterMaxOffset * LocalRng.FRandRange(-1, 1),
			Loc.Z
		});
	}
	if (const auto Curve = Sampling.ScaleDistributionCurve)
	{
		const auto Val = Curve->GetFloatValue(LocalRng.FRand());
		Transform.SetScale3D({Val, Val, Val});
	}
	if (Sampling.bRandomYaw)
	{
		Transform.SetRotation(FRotator(0, LocalRng.FRandRange(0, 360), 0).Quaternion());
	}
	return std::move(Transform);
}

FRandomStream BaseGenerator::GetLocalRng(const FGridPosition& Pos) const
{
	return GetLocalRng(GetTypeHash(Pos));
}

FRandomStream BaseGenerator::GetLocalRng(const uint32 Hash) const
{
	return {static_cast<int32>(HashCombineFast(Seed, Hash))};
}
