#include "BiomeTypeCustomization.h"

TSharedRef<IDetailCustomization> FBiomeTypeCustomization::MakeInstance()
{
	return MakeShareable(new FBiomeTypeCustomization);
}

FColor FBiomeTypeCustomization::GenerateColor(const UBiomeType& Ctx, const FWorldGridPos& Pos) const
{
	return Generator.GetValue().IsEligible(Pos, Ctx) ? Ctx.Color.ToFColorSRGB() : FColor::Black;
}
