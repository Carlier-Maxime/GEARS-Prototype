#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "ThumbnailSaverSettings.generated.h"

USTRUCT(BlueprintType)
struct FThumbnailMaterialTheme
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Identity")
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, Category="Name")
	bool bUseCustomName = false;
	
	UPROPERTY(EditAnywhere, Category="Materials")
	TArray<TSoftObjectPtr<UMaterialInstance>> MaterialsOverrides;
	
	FString GetName() const;
	
protected:
	UPROPERTY(EditAnywhere, Category="Name", meta=(EditCondition = bUseCustomName))
	FName Name;
};

USTRUCT(BlueprintType)
struct FThumbnailGenerationRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(ContentDir))
	FDirectoryPath SourcePath;

	UPROPERTY(EditAnywhere)
	FString SubFolderDest;

	UPROPERTY(EditAnywhere)
	bool bRecursive = true;
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AdditionalThemes;
};

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
		return Index ? &MaterialThemes[*Index] : nullptr;
	}

protected:
	UPROPERTY(EditAnywhere, Config, Category = "Automation")
	TArray<FThumbnailMaterialTheme> MaterialThemes;
	
private:
	TMap<FGameplayTag, int32> ThemesMap;
};