#include "GridNoiseDetailCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

TSharedRef<IDetailCustomization> FGridNoiseDetailCustomization::MakeInstance()
{
	return MakeShareable(new FGridNoiseDetailCustomization);
}

void FGridNoiseDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	auto& Category = DetailBuilder.EditCategory("Biome Preview");
	Category.AddCustomRow(FText::FromString("Preview"));
}
