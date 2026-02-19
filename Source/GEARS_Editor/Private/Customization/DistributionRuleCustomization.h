#pragma once

#include "CoreMinimal.h"
#include "PreviewPropertyTypeCustomization.h"
#include "Grid/Generator/ResourceGenerator.h"
#include "Grid/Generator/Context/DistributionRule.h"

class FDistributionRuleCustomization : public TPreviewPropertyTypeCustomization<FDistributionRule, ResourceGenerator>
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
protected:
	virtual TArray<TFunction<FColor(FWorldGridPos)>> BuildRowPreviewFunctions(const int32 RowIndex, const FDistributionRule* Ctx) override;
};
