// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "AssetRegistry/AssetData.h"
#include "Thumbnails/AutoGenData.h"

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
	static void PrepareAutoThumbnails(UThumbnailSaverSettings* Settings);
	static void InitializePlaceholderTextures();
	static void AutoGenerateThumbnails(bool ForceGen = false);

private:
	inline static int32 ExtenderIndex = -1;
	inline static FDelegateHandle ExtenderHandle;
	
	inline static TArray<ThumbnailToTexture::FAutoGenData> AutoGenFromAssets;
};
