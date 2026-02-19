#include "BiomeTypeCustomization.h"

TSharedRef<IDetailCustomization> FBiomeTypeCustomization::MakeInstance()
{
	return MakeShareable(new FBiomeTypeCustomization);
}

TArray<TFunction<FColor(FWorldGridPos)>> FBiomeTypeCustomization::BuildRowPreviewFunctions(const int32 RowIndex,
	const UBiomeType* Ctx)
{
	return {
		[this, Ctx](FWorldGridPos Pos)->FColor
		{
			return Generator.GetValue().IsEligible(Pos, Ctx->Data) ? Ctx->Data.Color.ToFColorSRGB() : FColor::Black;
		}
	};
}
