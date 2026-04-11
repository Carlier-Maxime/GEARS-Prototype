// Fill out your copyright notice in the Description page of Project Settings.

#include "ThumbnailToTexture.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Settings/ThumbnailSaverSettings.h"
#include "Thumbnails/Factory.h"
#include "Thumbnails/PathUtils.h"

using namespace ThumbnailToTexture;

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
	const auto SavePath = PathUtils::GetTextureSavePathFor(AssetData.GetAsset());
	auto* Texture = Factory::MakeTextureFromExistingThumbnail(AssetData, CreatePackage(*SavePath));
	if (UThumbnailSaverSettings::GetRef().bAutoSaveOnDisk) Factory::SaveTexture(Texture);
	return Texture != nullptr;
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
			const auto SavePath = PathUtils::GetTextureSavePathFor(AssetData.GetAsset(), Rule);
			AutoGenFromAssets.Emplace(AssetData, SavePath);
			for (const auto& ThemeTag : Rule.AdditionalThemes)
			{
				const auto* Theme = Settings->FindTheme(ThemeTag);
				if (!Theme) continue;
				AutoGenFromAssets.Emplace(AssetData, SavePath+"_"+Theme->GetName(), &Theme->MaterialsOverrides);
			}
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
		if (!Factory::MakeTextureFrom(PlaceHolder, Res, Res, CreatePackage(*AutoGenData.SavePath))) continue;
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
		const bool bNeedGen = ForceGen || AutoGenData.NeedGenerate();
		TotalNeedGen += bNeedGen;
		if (!bNeedGen) continue;
		auto* Texture = Factory::MakeTextureByRenderThumbnail(AutoGenData);
		if (!Texture) continue;
		if (UThumbnailSaverSettings::GetRef().bAutoSaveOnDisk) Factory::SaveTexture(Texture);
		AutoGenData.LastGenTime = FDateTime::UtcNow();
		++TotalGenerated;
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
