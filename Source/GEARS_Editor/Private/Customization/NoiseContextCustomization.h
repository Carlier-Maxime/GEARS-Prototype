#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "PreviewContextCustomizationBase.h"
#include "Grid/Generator/BaseGenerator.h"
#include "Grid/Generator/Context/NoiseContext.h"

class FNoiseContextCustomization : public FPreviewContextCustomizationBase<FNoiseContext, BaseGenerator>
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
protected:
	virtual FColor GenerateColor(const FNoiseContext& Ctx, const FGridPosition& Pos) const override;
};
