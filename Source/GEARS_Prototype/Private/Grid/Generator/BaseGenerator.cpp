#include "BaseGenerator.h"

#include "Definitions/GEARS_Macro.h"
#include "Settings/GridParams.h"

constexpr float RngSeedMove = 100000.f;
constexpr float RngResourceMove = 10000.f;

BaseGenerator::BaseGenerator(int32 Seed) : BaseGenerator(Seed, true) {}

BaseGenerator::BaseGenerator(const int32 Seed, const bool bGenResourceOffset) :
		RngStream(Seed), SeedOffset(GetRandomSeedOffset())
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

int16 BaseGenerator::SampleResourceAtPosition(const FGridPosition& Pos) const
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
	return NoiseValue > Ctx.NoiseThreshold;
}
