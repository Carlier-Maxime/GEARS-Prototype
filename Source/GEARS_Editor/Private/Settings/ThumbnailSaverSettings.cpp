#include "Settings/ThumbnailSaverSettings.h"

#include "ContentBrowserExtensions/ThumbnailToTexture.h"

void UThumbnailSaverSettings::PostInitProperties()
{
	for (int32 i=0; i < MaterialThemes.Num(); ++i)
	{
		ThemesMap.Emplace(MaterialThemes[i].Tag, i);
	}
	FThumbnailContentBrowserExtensions_Impl::PrepareAutoThumbnails(this);
	Super::PostInitProperties();
}

FString FThumbnailMaterialTheme::GetName() const
{
	return (bUseCustomName && Name.GetStringLength()>0) ? Name.ToString() :  Tag.ToString().Replace(TEXT("."), TEXT("_"));
}
