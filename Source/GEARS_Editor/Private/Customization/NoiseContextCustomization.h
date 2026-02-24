#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "PreviewPropertyTypeCustomization.h"
#include "Grid/Generator/BaseGenerator.h"
#include "Grid/Generator/Context/NoiseContext.h"

class FNoiseContextCustomization : public TPreviewPropertyTypeCustomization<FNoiseContext, BaseGenerator>
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
protected:
	virtual TArray<FNoisePreviewState::FGenColorFn> BuildRowPreviewFunctions(const int32 RowIndex, const FNoiseContext* Ctx) override;
};
