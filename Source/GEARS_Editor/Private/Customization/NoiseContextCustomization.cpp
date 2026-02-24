#include "NoiseContextCustomization.h"


TSharedRef<IPropertyTypeCustomization> FNoiseContextCustomization::MakeInstance()
{
	return MakeShareable(new FNoiseContextCustomization);
}

TArray<FNoisePreviewState::FGenColorFn> FNoiseContextCustomization::BuildRowPreviewFunctions(const int32 RowIndex, const FNoiseContext* Ctx)
{
	return {
		[this, Ctx](FNoisePreviewState::FPixelWriteContext Pixel) -> FColor
		{
			const uint8 ColorVal = FMath::Floor(Generator.GetValue().GetNoiseDensity(Pixel.ToWorld(), *Ctx, FVector2D::ZeroVector) * 255.f);
			return {ColorVal, ColorVal, ColorVal, 255};
		}
	};
}
