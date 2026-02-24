#include "DistributionRuleCustomization.h"


TSharedRef<IPropertyTypeCustomization> FDistributionRuleCustomization::MakeInstance()
{
	return MakeShareable(new FDistributionRuleCustomization);
}

TArray<FNoisePreviewState::FGenColorFn> FDistributionRuleCustomization::BuildRowPreviewFunctions(const int32 RowIndex,
	const FDistributionRule* Ctx)
{
	return {
		[this, Ctx](FNoisePreviewState::FPixelWriteContext Pixel)->FColor
		{
			const uint8 ColorVal = Generator.GetValue().ShouldSpawn(Pixel.ToWorld(), *Ctx, FVector2D::ZeroVector) ? 255 : 0;
			return {ColorVal, ColorVal, ColorVal, 255};
		}
	};
}
