#include "NoiseContextCustomization.h"


TSharedRef<IPropertyTypeCustomization> FNoiseContextCustomization::MakeInstance()
{
	return MakeShareable(new FNoiseContextCustomization);
}

TArray<TFunction<FColor(FWorldGridPos)>> FNoiseContextCustomization::BuildRowPreviewFunctions(const int32 RowIndex, const FNoiseContext* Ctx)
{
	return {
		[this, Ctx](FWorldGridPos Pos) -> FColor
		{
			const uint8 ColorVal = FMath::Floor(Generator.GetValue().GetNoiseDensity(Pos, *Ctx, FVector2D::ZeroVector) * 255.f);
			return {ColorVal, ColorVal, ColorVal, 255};
		}
	};
}
