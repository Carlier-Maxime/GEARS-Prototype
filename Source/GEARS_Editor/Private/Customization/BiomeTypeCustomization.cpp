#include "BiomeTypeCustomization.h"

TSharedRef<IDetailCustomization> FBiomeTypeCustomization::MakeInstance()
{
	return MakeShareable(new FBiomeTypeCustomization);
}

TArray<FNoisePreviewState::FGenColorFn> FBiomeTypeCustomization::BuildRowPreviewFunctions(const int32 RowIndex,
	const UBiomeType* Ctx)
{
	return {
		[this, Ctx](FNoisePreviewState::FPixelWriteContext Pixel)->FColor
		{
			return Generator.GetValue().IsEligible(Pixel.ToWorld(), Ctx->Data) ? Ctx->Data.Color.ToFColorSRGB() : FColor::Black;
		},
		[this, Ctx](FNoisePreviewState::FPixelWriteContext Pixel)->FColor
		{
			const auto Climate = Pixel.ToNormalizedUV();
			return Generator.GetValue().IsEligible(Climate.X, Climate.Y, Ctx->Data) ? Ctx->Data.Color.ToFColorSRGB() : FColor::Black;
		}
	};
}
