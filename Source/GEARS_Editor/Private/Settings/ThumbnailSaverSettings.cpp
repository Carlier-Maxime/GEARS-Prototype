#include "Settings/ThumbnailSaverSettings.h"

#include "ContentBrowserExtensions/ThumbnailToTexture.h"

void UThumbnailSaverSettings::PostInitProperties()
{
	FThumbnailContentBrowserExtensions_Impl::PrepareAutoThumbnails(this);
	Super::PostInitProperties();
}
