#pragma once
#include "PreviewDetailCustomization.h"
#include "Grid/Generator/BiomeGenerator.h"
#include "Settings/GridSettings.h"

class FGridSettingsCustomization : public TPreviewDetailCustomization<UGridSettings, BiomeGenerator>
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	
protected:
	virtual TArray<TFunction<FColor(FWorldGridPos)>> BuildRowPreviewFunctions(const int32 RowIndex, const UGridSettings* Ctx) override;
};
