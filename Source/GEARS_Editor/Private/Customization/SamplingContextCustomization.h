#pragma once

#include "CoreMinimal.h"
#include "PreviewPropertyTypeCustomization.h"
#include "Grid/Generator/ResourceGenerator.h"
#include "Grid/Generator/Context/DistributionRule.h"

class FSamplingContextCustomization : public TPreviewPropertyTypeCustomization<FDistributionRule, ResourceGenerator>
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
protected:
	virtual FColor GenerateColor(const FDistributionRule& Ctx, const FWorldGridPos& Pos) const override;
};
