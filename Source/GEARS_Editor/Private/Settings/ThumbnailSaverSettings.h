#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "Thumbnails/ThumbnailMaterialTheme.h"
#include "Thumbnails/ThumbnailGenerationRule.h"
#include "ThumbnailSaverSettings.generated.h"

UCLASS(Config = Editor, DefaultConfig, meta = (DisplayName = "Thumbnail Saver"))
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
	
	UPROPERTY(EditAnywhere, Config, Category = "Automation",
	meta = (DisplayName = "AutoGen Path", ContentDir, ToolTip="This is the base path for automation generation"))
	FDirectoryPath BasePath = { "/Game/_Generated/Icons/Thumbnails/" };
	
	UPROPERTY(EditAnywhere, Config, Category = "Automation", meta=(ToolTip = "This is the root tag for the theme, used for filter tag in Editor, during choose a MaterialTheme Tag. If Tag None, no filtering applyed"))
	FGameplayTag ThemeRootTag;
	
	UPROPERTY(EditAnywhere, Config, Category = "Automation")
	TArray<FThumbnailGenerationRule> Rules;

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

	virtual void PostInitProperties() override;
	
	FORCEINLINE const FThumbnailMaterialTheme* FindTheme(const FGameplayTag& Tag) const
	{
		const auto* Index = ThemesMap.Find(Tag);
		if (!Index)
		{
			UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Invalid Theme Tag : %s"), *Tag.ToString());
			return nullptr;
		}
		return &MaterialThemes[*Index];
	}

protected:
	UPROPERTY(EditAnywhere, Config, Category = "Automation")
	TArray<FThumbnailMaterialTheme> MaterialThemes;
	
private:
	TMap<FGameplayTag, int32> ThemesMap;
};