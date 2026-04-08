// Fill out your copyright notice in the Description page of Project Settings.

#include "ThumbnailToTexture.h"



void FThumbnailContentBrowserExtensions_Impl::RegisterExtender(TArray<FContentBrowserMenuExtender_SelectedAssets>& Extenders)
{
	ExtenderIndex = Extenders.Emplace(FContentBrowserMenuExtender_SelectedAssets::CreateStatic(OnExtendContentBrowserAssetSelectionMenu));
	ExtenderHandle = Extenders[ExtenderIndex].GetHandle();
}

void FThumbnailContentBrowserExtensions_Impl::UnregisterExtender(
	TArray<FContentBrowserMenuExtender_SelectedAssets>& Extenders)
{
	if (Extenders.Num() > ExtenderIndex && ExtenderIndex >= 0 && Extenders[ExtenderIndex].GetHandle() == ExtenderHandle)
		Extenders.RemoveAt(ExtenderIndex);
	else Extenders.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Extender)
		{
			return Extender.GetHandle() == ExtenderHandle;
		});
}

TSharedRef<FExtender> FThumbnailContentBrowserExtensions_Impl::OnExtendContentBrowserAssetSelectionMenu(
	const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender = MakeShared<FExtender>();
	if (SelectedAssets.Num() == 0) return Extender;
	
	Extender->AddMenuExtension(
		"CommonAssetActions",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateLambda([SelectedAssets](FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.AddMenuEntry(
				NSLOCTEXT("AssetTypeActions", "ThumbnailToTexture_Label", "Export Thumbnail as Texture"),
				NSLOCTEXT("AssetTypeActions", "ThumbnailToTexture_Tooltip", "Save thumbnail of this asset to Texture2D"),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "ClassIcon.Texture2D"),
				FUIAction(FExecuteAction::CreateLambda([SelectedAssets]()
				{
					ExecuteForAssets(SelectedAssets);
				}))
			);
		})
	);
	return Extender;
}

void FThumbnailContentBrowserExtensions_Impl::ExecuteForAssets(const TArray<FAssetData>& Assets)
{
	UE_LOG(LogTemp, Warning, TEXT("ThumbnailToTexture: %d asset(s)"), Assets.Num()); // TODO
}
