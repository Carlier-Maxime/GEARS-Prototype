#pragma once

#include "Engine/DeveloperSettings.h"
#include "ThumbnailSaverSettings.generated.h"

UCLASS(Config = EditorPerProjectUserSettings, meta = (DisplayName = "Thumbnail Saver"))
class GEARS_EDITOR_API UThumbnailSaverSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UThumbnailSaverSettings()
	{
		CategoryName = "Editor";
		SectionName  = "Thumbnail Saver";
	}
	
	UPROPERTY(EditAnywhere, Config, Category = "Resolution",
		meta = (DisplayName = "Max Size", ClampMin = 16, UIMax = 512))
	int32 MaxThumbnailSize = 128;
	
	UPROPERTY(EditAnywhere, Config, Category = "Output")
	FString OutputPrefix = "T_";
	
	UPROPERTY(EditAnywhere, Config, Category = "Output")
	bool bAutoRemoveUpperPrefixInSource = true;
	
	UPROPERTY(EditAnywhere, Config, Category = "Output")
	bool bAutoSaveOnDisk = false;

	FORCEINLINE static const UThumbnailSaverSettings* Get()
	{
		return GetDefault<UThumbnailSaverSettings>();
	}
	
	FORCEINLINE static const UThumbnailSaverSettings& GetRef()
	{
		auto* Settings = Get();
		check(Settings);
		return *Settings;
	}
};