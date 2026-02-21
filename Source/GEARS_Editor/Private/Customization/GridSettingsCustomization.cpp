#include "GridSettingsCustomization.h"

#include "Data/Wrappers/BiomeType.h"
#include "Settings/GridParams.h"

TSharedRef<IDetailCustomization> FGridSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FGridSettingsCustomization);
}

TArray<TFunction<FColor(FWorldGridPos)>> FGridSettingsCustomization::BuildRowPreviewFunctions(const int32 RowIndex,
	const UGridSettings* Ctx)
{
	return {
		[this](FWorldGridPos Pos)->FColor
		{
			auto& Registry = GridParams::Get().GetBiomeRegistry();
			const auto Index = Generator.GetValue().SampleBiome(Pos);
			return Index==Registry.INVALID_INDEX ? FColor::Black : Registry[Index].Color.ToFColorSRGB();
		}
	};
}
