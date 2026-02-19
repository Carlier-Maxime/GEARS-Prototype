#include "DistributionRuleCustomization.h"


TSharedRef<IPropertyTypeCustomization> FDistributionRuleCustomization::MakeInstance()
{
	return MakeShareable(new FDistributionRuleCustomization);
}

TArray<TFunction<FColor(FWorldGridPos)>> FDistributionRuleCustomization::BuildRowPreviewFunctions(const int32 RowIndex,
	const FDistributionRule* Ctx)
{
	return {
		[this, Ctx](FWorldGridPos Pos)->FColor
		{
			const uint8 ColorVal = Generator.GetValue().ShouldSpawn(Pos, *Ctx, FVector2D::ZeroVector) ? 255 : 0;
			return {ColorVal, ColorVal, ColorVal, 255};
		}
	};
}
