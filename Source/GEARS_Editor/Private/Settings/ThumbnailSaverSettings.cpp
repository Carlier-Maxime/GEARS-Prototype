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
