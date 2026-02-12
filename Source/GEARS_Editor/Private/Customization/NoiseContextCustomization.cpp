#include "NoiseContextCustomization.h"


TSharedRef<IPropertyTypeCustomization> FNoiseContextCustomization::MakeInstance()
{
	return MakeShareable(new FNoiseContextCustomization);
}

FColor FNoiseContextCustomization::GenerateColor(const FNoiseContext& Ctx, const FGridPosition& Pos) const
{
	const uint8 ColorVal = FMath::Floor(Generator.GetValue().GetNoiseDensity(Pos, Ctx, FVector2D::ZeroVector) * 255.f);
	return {ColorVal, ColorVal, ColorVal, 255};
}
