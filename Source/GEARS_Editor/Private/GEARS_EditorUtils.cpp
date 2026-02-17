#include "GEARS_EditorUtils.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

void GEARSEditorUtils::HandleCreateAssetRequest(const FGeneratedAssetData& Data, UClass* AssetClass)
{
	if (!AssetClass) return;
	const auto PackagePath = Data.GetPackagePath();
	const auto AssetPath = Data.GetAssetPath();
	
	const auto Package = CreatePackage(*PackagePath);
	const auto NewAsset = NewObject<UObject>(Package, AssetClass, *Data.Name, RF_Public | RF_Standalone);
	FAssetRegistryModule::AssetCreated(NewAsset);
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	const auto Filename = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, NewAsset, *Filename, SaveArgs);
	Data.Asset = NewAsset;
	NewAsset->PreEditChange(nullptr);
	NewAsset->PostEditChange();
	UE_LOG(LogTemp, Log, TEXT("GEARS_Editor: Asset %s created successfully."), *AssetPath);
}
