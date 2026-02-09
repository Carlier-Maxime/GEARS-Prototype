#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "Preview/NoisePreviewCache.h"

class FGridNoiseStructCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	static FString GetUniqueID(const TSharedRef<IPropertyHandle>& PropertyHandle);
	static FNoisePreviewCache& CreateOrUpdateCache(const TSharedRef<IPropertyHandle>& PropertyHandle);
	static FNoisePreviewCache& CreateOrUpdateCache(const TSharedRef<IPropertyHandle>& PropertyHandle, const FString& UniqueID);
	static void ClearCache();

private:
	int32 ThumbnailSize = 128;
	
	static TMap<FString, FNoisePreviewCache> PreviewCache;
};
