#pragma once

#include "DetailWidgetRow.h"
#include "Grid/Types/WorldGridPos.h"
#include "Widgets/SNoisePreviewsWidget.h"

template<typename ContextType, typename GeneratorType>
class TPreviewCustomizationBase
{
public:
	virtual ~TPreviewCustomizationBase() = default;

protected:
	void AddPreviewRow(FDetailWidgetRow& Row, const TArray<TSharedRef<IPropertyHandle>>& Handles, const ContextType* Ctx)
	{
		auto Widget = SNew(SNoisePreviewsWidget)
	        .PropertyHandles(Handles)
	        .OnSeedChanged_Lambda([this](int32 Seed){Generator.Emplace(Seed);});
		const auto Index = Widgets.Add(Widget);
		for (auto Preview : BuildRowPreviewFunctions(Index, Ctx)) Widget->AddPreview(Preview);
		
		Row.WholeRowContent()
		[
			Widget
		];
	}
	
	virtual TArray<FNoisePreviewState::FGenColorFn> BuildRowPreviewFunctions(const int32 RowIndex, const ContextType* Ctx) = 0;
	
	TOptional<GeneratorType> Generator;
	
private:
	TArray<TSharedRef<SNoisePreviewsWidget>> Widgets;
};
