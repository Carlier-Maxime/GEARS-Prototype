#include "Grid/Generator/ResourceGenerator.h"

#include "Grid/Generator/ProcSpawnData.h"
#include "Grid/Generator/Context/DistributionRule.h"
#include "Settings/GridParams.h"

ResourceGenerator::ResourceGenerator(const int32 Seed) : BaseGenerator(Seed)
{
}

FProcSpawnData ResourceGenerator::Sample(const FWorldGridPos& Pos, const TArray<FResourceRule>& Rules) const
{
	FProcSpawnData SpawnData;
	SpawnData.ResourceTypeIndex = DetermineType(Pos, Rules);
	SpawnData.Transform = GetVariationTransform(Pos, SpawnData.ResourceTypeIndex);
	return std::move(SpawnData);
}

bool ResourceGenerator::PassesSpacingTest(const FWorldGridPos& Pos, const uint32 Spacing, int32 Salt) const
{
	if (!Spacing) return true;
	const auto Space = Spacing*2 + 1;
	const auto SafeRadius = FMath::Max<uint32>(1,Spacing - (Spacing >> 2));
	const int32 GridX = Pos.X >= 0 ? Pos.X / Space : (Pos.X - Space + 1) / Space;
	const int32 GridY = Pos.Y >= 0 ? Pos.Y / Space : (Pos.Y - Space + 1) / Space;
	const FIntPoint SpacingPos(GridX, GridY);
	const auto SpaceRng = GetLocalRng(HashCombineFast(Salt, GetTypeHash(SpacingPos)));
	const auto OffsetX = SpaceRng.RandRange(Spacing - SafeRadius, Spacing + SafeRadius);
	const auto OffsetY = SpaceRng.RandRange(Spacing - SafeRadius, Spacing + SafeRadius);
	int32 LocalX = Pos.X % Space; if (LocalX < 0) LocalX += Space;
	int32 LocalY = Pos.Y % Space; if (LocalY < 0) LocalY += Space;
	return LocalX == OffsetX && LocalY == OffsetY;
}

bool ResourceGenerator::ShouldSpawn(const FWorldGridPos& Pos, const FDistributionRule& Rule,
                                    const FVector2D& Offset) const
{
	if (!PassesSpacingTest(Pos, Rule.Spacing, GetTypeHash(Offset))) return false;
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

int16 ResourceGenerator::DetermineType(const FWorldGridPos& Pos, const TArray<FResourceRule>& Rules) const
{
	const auto& Registry = GridParams::Get().GetResourceRegistry();
	for (const auto& Rule : Rules)
	{
		if (!ShouldSpawn(Pos, Rule.Distribution, GetOffset(Rule.ResourceTag))) continue;
		const auto Tags = Registry.TagsOf(Rule.ResourceTag);
		const auto Index = Tags.Num()==1 ? 0 : GetLocalRng(Pos).RandRange(0, Tags.Num()-1);
		return Registry.GetIndex(Tags.GetByIndex(Index));
	}
	return -1;
}

FTransform ResourceGenerator::GetVariationTransform(const FWorldGridPos& Pos, int16 ResourceTypeIndex) const
{
	if (ResourceTypeIndex == -1) return Pos.ToTransform();
	const auto& PlacementRule = GridParams::Get().GetResourceRegistry()[ResourceTypeIndex].PlacementRule;
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
