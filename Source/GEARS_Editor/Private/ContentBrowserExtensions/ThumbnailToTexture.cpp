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
	auto* Asset = AssetData.GetAsset();
	if (!Asset)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Asset %s is not valid."), *AssetData.GetFullName());
		return false;
	}
	return MakeTextureFrom(AssetData, GenSavePathFrom(Asset));
}

bool FThumbnailContentBrowserExtensions_Impl::MakeTextureFrom(const FAssetData& AssetData, const FString& SavePath)
{
	FObjectThumbnail Thumbnail;
	ThumbnailTools::LoadThumbnailFromPackage(AssetData, Thumbnail);

	auto& Settings = UThumbnailSaverSettings::GetRef();
	const auto Width  = FMath::Min(Thumbnail.GetImageWidth(), Settings.MaxThumbnailSize);
	const auto Height = FMath::Min(Thumbnail.GetImageHeight(), Settings.MaxThumbnailSize);
	if (Width == 0 || Height == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to get thumbnail for %s."), *AssetData.GetFullName());
		return false;
	}
	
	return CreateAndSaveTexture(SavePath, Width, Height, Thumbnail.GetImage(), Settings.bAutoSaveOnDisk);
}

FString FThumbnailContentBrowserExtensions_Impl::GenSavePathFrom(const UObject* Asset)
{
	FString SourcePrefix, SourceName;
	auto& Settings = UThumbnailSaverSettings::GetRef();
	if (Settings.bAutoRemoveUpperPrefixInSource)
	{
		Asset->GetName().Split("_", &SourcePrefix, &SourceName);
		if (SourcePrefix.ToUpper() != SourcePrefix) SourceName = Asset->GetName();
	} else SourceName = Asset->GetName();
	auto TextureName = Settings.OutputPrefix + SourceName;
	FString PackageName = Asset->GetPackage()->GetName(); 
	FString DirectoryPath = FPaths::GetPath(PackageName); 
	return FPaths::Combine(DirectoryPath, TextureName);
}

bool FThumbnailContentBrowserExtensions_Impl::CreateAndSaveTexture(const FString& SavePath, int32 Width, int32 Height,
                                                                   const FImageView& ImageView, const bool bSave)
{
	static FImage Image;
	ImageView.CopyTo(Image, ImageView.Format, ImageView.GammaSpace);
	return CreateAndSaveTexture(SavePath, Width, Height, Image, bSave);
}

bool FThumbnailContentBrowserExtensions_Impl::CreateAndSaveTexture(const FString& SavePath, int32 Width, int32 Height,
	const FImage& Image, const bool bSave)
{
	auto* Texture = CreateTexture(SavePath, Width, Height, Image);
	if (bSave) SaveTexture(SavePath, Texture);
	return Texture != nullptr;
}

UTexture2D* FThumbnailContentBrowserExtensions_Impl::CreateTexture(const FString& PackagePath, int32 Width, int32 Height,
	const FImage& Image)
{
	static FImage DestImage(Width, Height, ERawImageFormat::BGRA8);
	Image.ResizeTo(DestImage, Width, Height, DestImage.Format, Image.GammaSpace);
	auto* Package = CreatePackage(*PackagePath);
	Package->FullyLoad();
	const auto AssetName = FPackageName::GetShortName(PackagePath);
	
	auto* Texture = NewObject<UTexture2D>(Package, *AssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	if (!Texture)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to create texture for %s."), *PackagePath);
		return nullptr;
	}
	
	Texture->Source.Init(Width, Height, 1, 1, TSF_BGRA8, DestImage.RawData.GetData());
	Texture->CompressionSettings = TC_BC7;
	Texture->MipGenSettings      = TMGS_NoMipmaps;
	Texture->LODGroup            = TEXTUREGROUP_UI;
	Texture->SRGB                = true;
	Texture->UpdateResource();
	
	FAssetRegistryModule::AssetCreated(Texture);
	if (!Package->MarkPackageDirty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ThumbnailToTexture: cannot be marked package dirty ! (%s)"), *PackagePath)
	}
	
	return Texture;
}

void FThumbnailContentBrowserExtensions_Impl::SaveTexture(const FString& SavePath, UTexture2D* Texture)
{
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

	UPackage::SavePackage(Texture->GetPackage(), Texture, *PackageFilename, SaveArgs);
}

void FThumbnailContentBrowserExtensions_Impl::PrepareAutoThumbnails(UThumbnailSaverSettings* Settings)
{
	auto& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	
	FPaths::NormalizeDirectoryName(Settings->BasePath.Path);
	
	TArray<FString> PathsToScan;
	PathsToScan.Reserve(Settings->Rules.Num());
	for (auto& Rule : Settings->Rules)
	{
		FPaths::NormalizeDirectoryName(Rule.SourcePath.Path);
		PathsToScan.Emplace(Rule.SourcePath.Path);
	}
	AssetRegistry.ScanPathsSynchronous(PathsToScan);

	AutoGenFromAssets.Reset();
	TArray<FAssetData> AssetList;
	for (const auto& Rule : Settings->Rules)
	{
		FARFilter Filter;
		Filter.ClassPaths.Add(UObject::StaticClass()->GetClassPathName());
		Filter.bRecursiveClasses = true;
		Filter.PackagePaths.Add(*Rule.SourcePath.Path);
		Filter.bRecursivePaths = Rule.bRecursive;

		AssetRegistry.GetAssets(Filter, AssetList);
		AutoGenFromAssets.Reserve(AutoGenFromAssets.Num() + AssetList.Num());

		for (const auto& AssetData : AssetList)
		{
			const auto FullPath = GenSavePathFrom(AssetData.GetAsset());
			auto Directory = FPaths::GetPath(FullPath);
			Directory.RightChopInline(Rule.SourcePath.Path.Len(), EAllowShrinking::No);
			Directory.InsertAt(0, *Settings->BasePath.Path);
			AutoGenFromAssets.Emplace(AssetData, FPaths::Combine(Directory, FPaths::GetBaseFilename(FullPath)));
		}
		AssetList.Reset();
	}

	InitializePlaceholderTextures();
}

void FThumbnailContentBrowserExtensions_Impl::InitializePlaceholderTextures()
{
	const auto& Settings = UThumbnailSaverSettings::GetRef();
	const auto Res = Settings.MaxThumbnailSize;
	static const FImage PlaceHolder(Res, Res, ERawImageFormat::BGRA8);
	
	int32 TotalGenerated = 0;
	int32 TotalNeed = 0;
	for (auto& AutoGenData : AutoGenFromAssets)
	{
		if (FPackageName::DoesPackageExist(AutoGenData.SavePath, nullptr)) continue;
		++TotalNeed;
		if (!CreateTexture(AutoGenData.SavePath, Res, Res, PlaceHolder)) continue;
		++TotalGenerated;
	}
	
	if (TotalNeed == 0) return;
	if (const int32 TotalFail = TotalNeed - TotalGenerated; TotalFail == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("ThumbnailToTexture: Successfully created PlaceHolder %d required."), TotalGenerated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ThumbnailToTexture: PlaceHolder creation completed with errors. (Success: %d, Fail: %d / Total needed: %d)"), 
			   TotalGenerated, TotalFail, TotalNeed);
	}
}

void FThumbnailContentBrowserExtensions_Impl::AutoGenerateThumbnails(const bool ForceGen)
{
	int32 TotalGenerated = 0;
	int32 TotalNeedGen = 0;
	for (auto& AutoGenData : AutoGenFromAssets)
	{
		const bool bNeedGen = ForceGen || NeedGenerate(AutoGenData);
		TotalNeedGen += bNeedGen;
		if (!bNeedGen) continue; 
		if (!MakeTextureFrom(AutoGenData.AssetData, AutoGenData.SavePath)) continue;
		++TotalGenerated;
		AutoGenData.LastGenTime = FDateTime::UtcNow();
	}
	
	if (TotalNeedGen == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("ThumbnailToTexture: All textures are up to date."));
		return;
	}
	if (const int32 TotalFailGen = TotalNeedGen - TotalGenerated; TotalFailGen == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("ThumbnailToTexture: Successfully updated all %d required texture(s)."), TotalGenerated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ThumbnailToTexture: Update completed with errors. (Success: %d, Fail: %d / Total needed: %d)"), 
			   TotalGenerated, TotalFailGen, TotalNeedGen);
	}
}

bool FThumbnailContentBrowserExtensions_Impl::NeedGenerate(const FAutoGenData& AutoGenData)
{
	FString FilePath;
	if (!FPackageName::DoesPackageExist(AutoGenData.SavePath, &FilePath)) return true;
	const auto TextureTime = IFileManager::Get().GetTimeStamp(*FilePath);
	if (!FPackageName::DoesPackageExist(AutoGenData.AssetData.PackageName.ToString(), &FilePath)) return false;
	const auto AssetTime = IFileManager::Get().GetTimeStamp(*FilePath);
	return AssetTime > (TextureTime > AutoGenData.LastGenTime ? TextureTime : AutoGenData.LastGenTime);
}
