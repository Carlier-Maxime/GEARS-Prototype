#pragma once

#include "CoreMinimal.h"
#include "PreviewContextCustomizationBase.h"
#include "Grid/Generator/ResourceGenerator.h"
#include "Grid/Generator/Context/SamplingContext.h"

class FSamplingContextCustomization : public FPreviewContextCustomizationBase<FSamplingContext, ResourceGenerator>
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
protected:
	virtual FColor GenerateColor(const FSamplingContext& Ctx, const FGridPosition& Pos) const override;
};
