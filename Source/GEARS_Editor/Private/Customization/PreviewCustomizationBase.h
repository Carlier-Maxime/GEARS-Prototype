#pragma once

#include "DetailWidgetRow.h"
#include "Grid/Types/WorldGridPos.h"
#include "Widgets/SNoisePreviewWidget.h"

template<typename ContextType, typename GeneratorType>
class TPreviewCustomizationBase
{
public:
	virtual ~TPreviewCustomizationBase() = default;

protected:
	void AddPreviewRow(FDetailWidgetRow& Row, const TArray<TSharedRef<IPropertyHandle>>& Handles, const ContextType* Ctx)
	{
		Row.WholeRowContent()
		[
			SNew(SNoisePreviewWidget)
			.PropertyHandles(Handles)
			.OnSeedChanged_Lambda([this](int32 Seed){Generator.Emplace(Seed);})
			.OnGenerateColor_Lambda([this, Ctx](const FWorldGridPos& Pos)->FColor
			{
				return GenerateColor(*Ctx, Pos);
			})
		];
	}
	
	virtual FColor GenerateColor(const ContextType& Ctx, const FWorldGridPos& Pos) const = 0;
	TOptional<GeneratorType> Generator;
};
