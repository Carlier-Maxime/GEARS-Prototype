#include "ThumbnailMaterialThemeCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Settings/ThumbnailSaverSettings.h"

TSharedRef<IPropertyTypeCustomization> FThumbnailMaterialThemeCustomization::MakeInstance()
{
	return MakeShareable(new FThumbnailMaterialThemeCustomization);
}

void FThumbnailMaterialThemeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                           FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	auto TagHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FThumbnailMaterialTheme, Tag));
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

void FThumbnailMaterialThemeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	for (uint32 i=0; i<NumChildren; ++i)
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
	}
}
