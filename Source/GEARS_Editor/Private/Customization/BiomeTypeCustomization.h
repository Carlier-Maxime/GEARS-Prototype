#pragma once
#include "PreviewDetailCustomization.h"
#include "Data/Wrappers/BiomeType.h"
#include "Grid/Generator/BiomeGenerator.h"

class FBiomeTypeCustomization : public TPreviewDetailCustomization<UBiomeType, BiomeGenerator>
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	
protected:
	virtual FColor GenerateColor(const UBiomeType& Ctx, const FWorldGridPos& Pos) const override;
};
