#include "GridSettingsCustomization.h"

#include "Data/Wrappers/BiomeType.h"
#include "Settings/GridParams.h"

TSharedRef<IDetailCustomization> FGridSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FGridSettingsCustomization);
}

FColor FGridSettingsCustomization::GenerateColor(const UGridSettings& Ctx, const FWorldGridPos& Pos) const
{
	const auto Index = Generator.GetValue().SampleBiome(Pos);
	return Index==-1 ? FColor::Black : GridParams::Get().GetBiomeRegistry()[Index].Get()->Data.Color.ToFColorSRGB();
}
