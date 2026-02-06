#include "BaseGenerator.h"

#include "ProcSpawnData.h"
#include "Definitions/GEARS_Macro.h"
#include "Settings/GridParams.h"

constexpr float RngSeedMove = 1024.f;
constexpr float RngResourceMove = 256.f;

BaseGenerator::BaseGenerator(const int32 Seed) : BaseGenerator(Seed, true) {}

BaseGenerator::BaseGenerator(const int32 Seed, const bool bGenResourceOffset) :
		Seed(Seed), RngStream(Seed), SeedOffset(GetRandomSeedOffset())
{
	if (!bGenResourceOffset) return;
	for (auto SoftResource : GridParams::Get().GetResourceRegistry())
	{
		if (!ensureSoftPtr(SoftResource)) continue;
		const auto Resource = SoftResource.LoadSynchronous();
		Resource->Sampling.Offset = GetRandomResourceOffset();
	}
}

FVector2D BaseGenerator::GetRandomResourceOffset() const
{
	return GetRandomOffset(RngResourceMove);
}

FVector2D BaseGenerator::GetRandomSeedOffset() const
{
	return GetRandomOffset(RngSeedMove);
}

FVector2D BaseGenerator::GetRandomOffset(const float Displacement) const
{
	return {
		RngStream.FRandRange(-Displacement, Displacement),
		RngStream.FRandRange(-Displacement, Displacement)
	};
}

FProcSpawnData BaseGenerator::SampleResourceAtPosition(const FGridPosition& Pos) const
{
	FProcSpawnData SpawnData;
	SpawnData.ResourceTypeIndex = DetermineResourceType(Pos);
	if (SpawnData.ResourceTypeIndex == -1) goto exit;
	SpawnData.Transform = Pos.ToTransform();
	
	exit:
	return std::move(SpawnData);
}

int16 BaseGenerator::DetermineResourceType(const FGridPosition& Pos) const
{
	const auto& Registry = GridParams::Get().GetResourceRegistry();
	for (auto i=0; i<Registry.Num(); ++i)
	{
		if (!ensureSoftPtr(Registry[i])) continue;
		if (ShouldSpawnResource(Pos, Registry[i].LoadSynchronous()->Sampling)) return i;
	}
	return -1;
}

bool BaseGenerator::ShouldSpawnResource(const FGridPosition& Pos, const FSamplingContext& Ctx) const
{
	const auto NoiseValue = (FMath::PerlinNoise2D({
		(Pos.X + SeedOffset.X + Ctx.Offset.X) * Ctx.NoiseScale,
		(Pos.Y + SeedOffset.Y + Ctx.Offset.Y) * Ctx.NoiseScale
	}) + 1.0f) * 0.5f;
	if (Ctx.ThresholdSmoothing == 0) return NoiseValue >= Ctx.NoiseThreshold;
	const float SpawnChance = FMath::SmoothStep(
		Ctx.NoiseThreshold,
		Ctx.NoiseThreshold + Ctx.ThresholdSmoothing, 
		NoiseValue
	);
	if (SpawnChance == 0) return false;
	if (SpawnChance == 1) return true;
	const FRandomStream LocalRng(HashCombineFast(Seed, GetTypeHash(Pos.GetGridPos())));
	return LocalRng.FRand() < SpawnChance;
}
