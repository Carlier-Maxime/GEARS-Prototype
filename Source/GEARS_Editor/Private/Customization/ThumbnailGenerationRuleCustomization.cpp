#include "ThumbnailGenerationRuleCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Settings/ThumbnailSaverSettings.h"

TSharedRef<IPropertyTypeCustomization> FThumbnailGenerationRuleCustomization::MakeInstance()
{
	return MakeShareable(new FThumbnailGenerationRuleCustomization);
}

void FThumbnailGenerationRuleCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	auto TagHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FThumbnailGenerationRule, AdditionalThemes));
	if (TagHandle.IsValid())
	{
		const auto RootTag = UThumbnailSaverSettings::GetRef().ThemeRootTag.ToString();
		TagHandle->SetInstanceMetaData(TEXT("Categories"), RootTag);
		TagHandle->GetProperty()->SetMetaData(TEXT("Categories"), *RootTag);
	}
	
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FThumbnailGenerationRuleCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	for (uint32 i=0; i<NumChildren; ++i)
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
	}
}
