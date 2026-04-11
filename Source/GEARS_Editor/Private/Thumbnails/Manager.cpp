#include "Manager.h"

#include "Factory.h"
#include "PathUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Settings/ThumbnailSaverSettings.h"

ThumbnailToTexture::FManager& ThumbnailToTexture::FManager::Get()
{
	static FManager Instance;
	return Instance;
}

ThumbnailToTexture::FManager::FManager() : 
		Res(UThumbnailSaverSettings::GetRef().MaxThumbnailSize),
		PlaceHolder(Res, Res, ERawImageFormat::BGRA8)
{
}

ThumbnailToTexture::FManager::~FManager()
{
}

void ThumbnailToTexture::FManager::InitializePlaceholderTextures()
{
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

void ThumbnailToTexture::FManager::PrepareAutoThumbnails(UThumbnailSaverSettings* Settings)
{
	if (!Settings) return;
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

void ThumbnailToTexture::FManager::AutoGenerateThumbnails(bool ForceGen)
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
