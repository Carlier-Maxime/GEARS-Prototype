#include "Settings/ThumbnailSaverSettings.h"

#include "Thumbnails/Manager.h"


void UThumbnailSaverSettings::PostInitProperties()
{
	for (int32 i=0; i < MaterialThemes.Num(); ++i)
	{
		ThemesMap.Emplace(MaterialThemes[i].Tag, i);
	}
	ThumbnailToTexture::FManager::Get().PrepareAutoThumbnails(this);
	Super::PostInitProperties();
}
