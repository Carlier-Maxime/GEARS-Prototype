#include "BaseGenerator.h"

#include "Settings/GridParams.h"

constexpr float RngSeedMove = 100000.f;
constexpr float RngResourceMove = 10000.f;

BaseGenerator::BaseGenerator(const int32 Seed) :
		RngStream(Seed), SeedOffset(GetRandomSeedOffset())
{
	const auto Registry = GridParams::Get().GetResourceRegistry();
	ResourcesOffset.Reserve(Registry.Num());
	for (auto i=0; i<Registry.Num(); ++i) ResourcesOffset.Add(GetRandomResourceOffset());
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

int16 BaseGenerator::SampleResourceAtPosition(const FGridPosition& Pos)
{
	for (auto i=0; i<ResourcesOffset.Num(); ++i)
	{
		const auto Resource = GridParams::Get().GetResourceRegistry()[i].LoadSynchronous();
		const auto ResourceOffset = ResourcesOffset[i];
		if (ShouldSpawnResource(Pos, *Resource, ResourceOffset)) return i;
	}
	return -1;
}

bool BaseGenerator::ShouldSpawnResource(const FGridPosition& Pos, const UResourceType& Resource) const
{
	return ShouldSpawnResource(Pos, Resource, FVector2D::ZeroVector);
}

bool BaseGenerator::ShouldSpawnResource(const FGridPosition& Pos, const UResourceType& Resource, const FVector2D& ResourceOffset) const
{
	const auto NoiseValue = (FMath::PerlinNoise2D({
		(Pos.X + SeedOffset.X + ResourceOffset.X) * Resource.NoiseScale,
		(Pos.Y + SeedOffset.Y + ResourceOffset.Y) * Resource.NoiseScale
	}) + 1.0f) * 0.5f;
	return NoiseValue > Resource.NoiseThreshold;
}
