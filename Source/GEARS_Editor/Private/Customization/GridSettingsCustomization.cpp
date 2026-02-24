#include "GridSettingsCustomization.h"

#include "Data/Wrappers/BiomeType.h"
#include "Settings/GridParams.h"

TSharedRef<IDetailCustomization> FGridSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FGridSettingsCustomization);
}

TArray<FNoisePreviewState::FGenColorFn> FGridSettingsCustomization::BuildRowPreviewFunctions(const int32 RowIndex,
	const UGridSettings* Ctx)
{
	return {
		[this](FNoisePreviewState::FPixelWriteContext Pixel)->FColor
		{
			auto& Registry = GridParams::Get().GetBiomeRegistry();
			const auto Index = Generator.GetValue().SampleBiome(Pixel.ToWorld());
			return Index==Registry.INVALID_INDEX ? FColor::Black : Registry[Index].Color.ToFColorSRGB();
		},
		[this](FNoisePreviewState::FPixelWriteContext Pixel)->FColor
		{
			auto& Registry = GridParams::Get().GetBiomeRegistry();
			const auto Climate = Pixel.ToNormalizedUV();
			const auto Index = Generator.GetValue().SampleBiome(Climate.X, Climate.Y);
			return Index==Registry.INVALID_INDEX ? FColor::Black : Registry[Index].Color.ToFColorSRGB();
		}
	};
}
