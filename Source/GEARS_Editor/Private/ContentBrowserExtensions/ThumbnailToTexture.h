// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "AssetRegistry/AssetData.h"

class UThumbnailSaverSettings;
/**
 * 
 */
class GEARS_EDITOR_API FThumbnailContentBrowserExtensions_Impl
{
public:
	static void RegisterExtender(TArray<FContentBrowserMenuExtender_SelectedAssets>& Extenders);
	static void UnregisterExtender(TArray<FContentBrowserMenuExtender_SelectedAssets>& Extenders);
	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets);
	static void ExecuteForAssets(const TArray<FAssetData>& Assets);
	static bool ExecuteForAsset(const FAssetData& AssetData);
	static FString GenSavePathFrom(const UObject* Asset);
	static bool MakeTextureFrom(const FAssetData& AssetData, const FString& SavePath);
	static bool CreateAndSaveTexture(const FString& SavePath, int32 Width, int32 Height, const FImageView& ImageView, bool bSave = true);
	static bool CreateAndSaveTexture(const FString& SavePath, int32 Width, int32 Height, const FImage& Image, bool bSave = true);
	static UTexture2D* CreateTexture(const FString& PackagePath, int32 Width, int32 Height, const FImage& Image);
	static void SaveTexture(const FString& SavePath, UTexture2D* Texture);
	static void PrepareAutoThumbnails(UThumbnailSaverSettings* Settings);
	static void AutoGenerateThumbnails(bool ForceGen = false);

private:
	struct FAutoGenData
	{
		FAssetData AssetData;
		FString SavePath;
		FDateTime LastGenTime = FDateTime::MinValue();
	};
	
	static bool NeedGenerate(const FAutoGenData& AutoGenData);
	
	inline static int32 ExtenderIndex = -1;
	inline static FDelegateHandle ExtenderHandle;
	
	inline static TArray<FAutoGenData> AutoGenFromAssets;
};
