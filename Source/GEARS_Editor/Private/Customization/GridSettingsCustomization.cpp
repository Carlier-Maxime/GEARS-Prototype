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
			const auto Index = Generator.GetValue().SampleBiome(Pos);
			return Index==-1 ? FColor::Black : GridParams::Get().GetBiomeRegistry()[Index].Color.ToFColorSRGB();
		}
	};
}
