#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "PreviewCustomizationBase.h"

template<typename ContextType, typename GeneratorType>
class TPreviewPropertyTypeCustomization : public TPreviewCustomizationBase<ContextType, GeneratorType>, public IPropertyTypeCustomization
{
public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		];
	}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		uint32 NumChildren;
		PropertyHandle->GetNumChildren(NumChildren);
	
		for (uint32 i=0; i<NumChildren; ++i)
		{
			ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
		}
	
		void* StructPtr = nullptr;
		PropertyHandle->GetValueData(StructPtr);
		auto Ctx = static_cast<ContextType*>(StructPtr);
	
		IDetailGroup& PreviewGroup = ChildBuilder.AddGroup(FName("PreviewSection"), FText::FromString("Preview"));
		TPreviewCustomizationBase<ContextType,GeneratorType>::AddPreviewRow(PreviewGroup.AddWidgetRow(), {PropertyHandle}, Ctx);
	}
};
