#pragma once

#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "PreviewCustomizationBase.h"

template<typename ContextType, typename GeneratorType>
class TPreviewDetailCustomization : public TPreviewCustomizationBase<ContextType, GeneratorType>, public IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TArray<TWeakObjectPtr<>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		auto MyAsset = Cast<ContextType>(Objects[0].Get());
		
		TArray<TSharedRef<IPropertyHandle>> AllHandles;
		for (TFieldIterator<FProperty> It(ContextType::StaticClass()); It; ++It)
		{
			TSharedRef<IPropertyHandle> Handle = DetailBuilder.GetProperty(It->GetFName());
			if (!Handle->IsValidHandle()) continue;
			AllHandles.Add(Handle);
		}
	
		auto& Category = DetailBuilder.EditCategory("Preview", FText::GetEmpty(), ECategoryPriority::Important);
		TPreviewCustomizationBase<ContextType,GeneratorType>::AddPreviewRow(Category.AddCustomRow(FText::FromString("")), AllHandles, MyAsset);
	}
};
