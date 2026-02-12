#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Widgets/SNoisePreviewWidget.h"
#include "Grid/Types/GridPosition.h"

template<typename ContextType, typename GeneratorType>
class FPreviewContextCustomizationBase : public IPropertyTypeCustomization
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
	
		IDetailGroup& PreviewGroup = ChildBuilder.AddGroup(FName("PreviewSection"), FText::FromString("Preview"));
		PreviewGroup.AddWidgetRow()
			.WholeRowContent()
			[
				SNew(SNoisePreviewWidget)
				.PropertyHandles({PropertyHandle})
				.OnSeedChanged_Lambda([this](int32 Seed){Generator.Emplace(Seed);})
				.OnGenerateColor_Lambda([this, StructPtr](const FGridPosition& Pos)->FColor
				{
					return GenerateColor(*static_cast<ContextType*>(StructPtr), Pos);
				})
			];
	
		PreviewGroup.ToggleExpansion(true);
	}

protected:
	virtual FColor GenerateColor(const ContextType& Ctx, const FGridPosition& Pos) const = 0;
	TOptional<GeneratorType> Generator;
};
