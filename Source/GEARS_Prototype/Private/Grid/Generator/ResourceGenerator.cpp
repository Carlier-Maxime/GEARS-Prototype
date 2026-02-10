#include "Grid/Generator/ResourceGenerator.h"

#include "Grid/Generator/ProcSpawnData.h"
#include "Definitions/GEARS_Macro.h"
#include "Grid/Generator/Context/SamplingContext.h"
#include "Settings/GridParams.h"

ResourceGenerator::ResourceGenerator(const int32 Seed) : BaseGenerator(Seed)
{
}

FProcSpawnData ResourceGenerator::Sample(const FGridPosition& Pos) const
{
	FProcSpawnData SpawnData;
	SpawnData.ResourceTypeIndex = DetermineType(Pos);
	SpawnData.Transform = GetVariationTransform(Pos, SpawnData.ResourceTypeIndex);
	return std::move(SpawnData);
}

bool ResourceGenerator::ShouldSpawn(const FGridPosition& Pos, const FSamplingContext& Ctx,
	const FVector2D& Offset) const
{
	const auto NoiseDensity = GetNoiseDensity(Pos, Ctx.Noise, Offset);
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

int16 ResourceGenerator::DetermineType(const FGridPosition& Pos) const
{
	const auto& Registry = GridParams::Get().GetResourceRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (!ensureSoftPtr(Registry[i])) continue;
		const auto& Sampling = Registry[i].LoadSynchronous()->Sampling;
		if (ShouldSpawn(Pos, Sampling, GetOffset(Registry[i]))) return i;
	}
	return -1;
}

FTransform ResourceGenerator::GetVariationTransform(const FGridPosition& Pos, int16 ResourceTypeIndex) const
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
