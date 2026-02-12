#pragma once
#include "PreviewDetailCustomization.h"
#include "Grid/Generator/BiomeGenerator.h"
#include "Settings/GridSettings.h"

class FGridSettingsCustomization : public TPreviewDetailCustomization<UGridSettings, BiomeGenerator>
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	
protected:
	virtual FColor GenerateColor(const UGridSettings& Ctx, const FGridPosition& Pos) const override;
};
