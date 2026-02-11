#include "GridNoiseStructCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Widgets/SNoisePreviewWidget.h"


TSharedRef<IPropertyTypeCustomization> FGridNoiseStructCustomization::MakeInstance()
{
	return MakeShareable(new FGridNoiseStructCustomization);
}

void FGridNoiseStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FGridNoiseStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	
	for (uint32 i=0; i<NumChildren; ++i)
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
	}
	
	void* StructPtr = nullptr;
	PropertyHandle->GetValueData(StructPtr);
	const UScriptStruct* StructType = CastField<FStructProperty>(PropertyHandle->GetProperty())->Struct;
	
	IDetailGroup& PreviewGroup = ChildBuilder.AddGroup(FName("PreviewSection"), FText::FromString("Preview"));
	PreviewGroup.AddWidgetRow()
		.WholeRowContent()
		[
			SNew(SNoisePreviewWidget).StructPtr(StructPtr).StructName(StructType->GetFName()).PropertyHandle(PropertyHandle)
		];
	
	PreviewGroup.ToggleExpansion(true);
}
