// Fill out your copyright notice in the Description page of Project Settings.

#include "ThumbnailToTexture.h"

#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Settings/ThumbnailSaverSettings.h"
#include "UObject/SavePackage.h"


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
	uint32 NumExecuted = 0;
	for (const FAssetData& Asset : Assets) NumExecuted += ExecuteForAsset(Asset);
	UE_LOG(LogTemp, Log, TEXT("ThumbnailToTexture: Request %d asset(s), succeed %d asset(s)."), Assets.Num(), NumExecuted);
}

bool FThumbnailContentBrowserExtensions_Impl::ExecuteForAsset(const FAssetData& AssetData)
{
	auto& Settings = UThumbnailSaverSettings::GetRef();
	auto* Asset = AssetData.GetAsset();
	if (!Asset)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Asset %s is not valid."), *AssetData.GetFullName());
		return false;
	}

	FObjectThumbnail Thumbnail;
	ThumbnailTools::LoadThumbnailFromPackage(AssetData, Thumbnail);

	const auto Width  = FMath::Min(Thumbnail.GetImageWidth(), Settings.MaxThumbnailSize);
	const auto Height = FMath::Min(Thumbnail.GetImageHeight(), Settings.MaxThumbnailSize);
	if (Width == 0 || Height == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to get thumbnail for %s."), *Asset->GetFullName());
		return false;
	}
	
	FString SourcePrefix, SourceName; 
	if (Settings.bAutoRemoveUpperPrefixInSource)
	{
		Asset->GetName().Split("_", &SourcePrefix, &SourceName);
		if (SourcePrefix.ToUpper() != SourcePrefix) SourceName = Asset->GetName();
	} else SourceName = Asset->GetName();
	auto TextureName = Settings.SavePrefix + SourceName;
	FString PackageName = AssetData.PackageName.ToString(); 
	FString DirectoryPath = FPaths::GetPath(PackageName); 
	const auto SavePath = FPaths::Combine(DirectoryPath, TextureName);
	
	return CreateAndSaveTexture(SavePath, Width, Height, Thumbnail.GetImage());
}

bool FThumbnailContentBrowserExtensions_Impl::CreateAndSaveTexture(const FString& SavePath, int32 Width, int32 Height,
	const FImageView& ImageView)
{
	static FImage Image;
	ImageView.CopyTo(Image, ImageView.Format, ImageView.GammaSpace);
	return CreateAndSaveTexture(SavePath, Width, Height, Image);
}

bool FThumbnailContentBrowserExtensions_Impl::CreateAndSaveTexture(const FString& SavePath, int32 Width, int32 Height,
	const FImage& Image)
{
	static FImage DestImage(Width, Height, ERawImageFormat::BGRA8);
	Image.ResizeTo(DestImage, Width, Height, DestImage.Format, Image.GammaSpace);
	auto* Package = CreatePackage(*SavePath);
	Package->FullyLoad();
	const auto AssetName = FPackageName::GetShortName(SavePath);
	
	auto* Texture = NewObject<UTexture2D>(Package, *AssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	Texture->Source.Init(Width, Height, 1, 1, TSF_BGRA8, DestImage.RawData.GetData());
	Texture->CompressionSettings = TC_BC7;
	Texture->MipGenSettings      = TMGS_NoMipmaps;
	Texture->LODGroup            = TEXTUREGROUP_UI;
	Texture->SRGB                = true;
	Texture->UpdateResource();
	
	FAssetRegistryModule::AssetCreated(Texture);
	if (!Package->MarkPackageDirty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ThumbnailToTexture: Texture cannot be marked package dirty ! (%s)"), *SavePath)
	}
	
	const FString PackageFilename =
		FPackageName::LongPackageNameToFilename(
			SavePath,
			FPackageName::GetAssetPackageExtension()
		);

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.Error         = GError;
	SaveArgs.bForceByteSwapping = false;
	SaveArgs.bWarnOfLongFilename = true;
	SaveArgs.SaveFlags = SAVE_NoError;

	UPackage::SavePackage(Package, Texture, *PackageFilename, SaveArgs);
	return true;
}
