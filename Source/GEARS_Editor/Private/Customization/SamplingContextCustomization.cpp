#include "SamplingContextCustomization.h"


TSharedRef<IPropertyTypeCustomization> FSamplingContextCustomization::MakeInstance()
{
	return MakeShareable(new FSamplingContextCustomization);
}

FColor FSamplingContextCustomization::GenerateColor(const FDistributionRule& Ctx, const FWorldGridPos& Pos) const
{
	const uint8 ColorVal = Generator.GetValue().ShouldSpawn(Pos, Ctx, FVector2D::ZeroVector) ? 255 : 0;
	return {ColorVal, ColorVal, ColorVal, 255};
}
