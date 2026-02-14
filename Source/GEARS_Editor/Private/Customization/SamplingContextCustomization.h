#pragma once

#include "CoreMinimal.h"
#include "PreviewPropertyTypeCustomization.h"
#include "Grid/Generator/ResourceGenerator.h"
#include "Grid/Generator/Context/SamplingContext.h"

class FSamplingContextCustomization : public TPreviewPropertyTypeCustomization<FSamplingContext, ResourceGenerator>
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
protected:
	virtual FColor GenerateColor(const FSamplingContext& Ctx, const FWorldGridPos& Pos) const override;
};
