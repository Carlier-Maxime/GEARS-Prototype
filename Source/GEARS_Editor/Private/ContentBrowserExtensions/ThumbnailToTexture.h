// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "AssetRegistry/AssetData.h"

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
private:
	inline static int32 ExtenderIndex = -1;
	inline static FDelegateHandle ExtenderHandle;
};
