#include "Grid/Generator/ResourceGenerator.h"

#include "Grid/Generator/ProcSpawnData.h"
#include "Grid/Generator/Context/DistributionRule.h"
#include "Settings/GridParams.h"
#include "Data/Wrappers/ResourceType.h"

ResourceGenerator::ResourceGenerator(const int32 Seed) : BaseGenerator(Seed)
{
}

FProcSpawnData ResourceGenerator::Sample(const FWorldGridPos& Pos) const
{
	FProcSpawnData SpawnData;
	SpawnData.ResourceTypeIndex = DetermineType(Pos);
	SpawnData.Transform = GetVariationTransform(Pos, SpawnData.ResourceTypeIndex);
	return std::move(SpawnData);
}

bool ResourceGenerator::ShouldSpawn(const FWorldGridPos& Pos, const FDistributionRule& Rule,
	const FVector2D& Offset) const
{
	const auto NoiseDensity = GetNoiseDensity(Pos, Rule.Noise, Offset);
	if (Rule.ThresholdSmoothing == 0) return NoiseDensity >= Rule.NoiseThreshold;
	const float SpawnChance = FMath::SmoothStep(
		Rule.NoiseThreshold,
		Rule.NoiseThreshold + Rule.ThresholdSmoothing, 
		NoiseDensity
	);
	if (SpawnChance <= 0.f) return false;
	if (SpawnChance >= 1.f) return true;
	return GetLocalRng(Pos).FRand() < SpawnChance;
}

int16 ResourceGenerator::DetermineType(const FWorldGridPos& Pos) const
{
	const auto& Registry = GridParams::Get().GetResourceRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (ShouldSpawn(Pos, Registry[i]->Data.Distribution, GetOffset(Registry[i]))) return i;
	}
	return -1;
}

FTransform ResourceGenerator::GetVariationTransform(const FWorldGridPos& Pos, int16 ResourceTypeIndex) const
{
	if (ResourceTypeIndex == -1) return Pos.ToTransform();
	const auto& PlacementRule = GridParams::Get().GetResourceRegistry()[ResourceTypeIndex]->Data.PlacementRule;
	return GetVariationTransform(Pos, PlacementRule);
}

FTransform ResourceGenerator::GetVariationTransform(const FWorldGridPos& Pos, const FPlacementRule& Rule) const
{
	auto Transform = Pos.ToTransform();
	const auto LocalRng = GetLocalRng(Pos);
	
	if (Rule.JitterMaxOffset > 0)
	{
		const auto Loc = Transform.GetLocation();
		Transform.SetLocation({
			Loc.X + GridParams::Get().GetCellSize() * Rule.JitterMaxOffset * LocalRng.FRandRange(-1, 1),
			Loc.Y + GridParams::Get().GetCellSize() * Rule.JitterMaxOffset * LocalRng.FRandRange(-1, 1),
			Loc.Z
		});
	}
	
	if (Rule.ScaleCurve)
	{
		const auto Val = Rule.ScaleCurve->GetFloatValue(LocalRng.FRand());
		Transform.SetScale3D({Val, Val, Val});
	}
	
	if (Rule.bRandomYaw)
	{
		Transform.SetRotation(FRotator(0, LocalRng.FRandRange(0, 360), 0).Quaternion());
	}
	return std::move(Transform);
}
