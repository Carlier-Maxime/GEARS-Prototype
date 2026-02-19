#pragma once
#include "PreviewDetailCustomization.h"
#include "Data/Wrappers/BiomeType.h"
#include "Grid/Generator/BiomeGenerator.h"

class FBiomeTypeCustomization : public TPreviewDetailCustomization<UBiomeType, BiomeGenerator>
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	
protected:
	virtual TArray<TFunction<FColor(FWorldGridPos)>> BuildRowPreviewFunctions(const int32 RowIndex, const UBiomeType* Ctx) override;
};
