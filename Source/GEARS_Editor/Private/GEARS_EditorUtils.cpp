#include "GEARS_EditorUtils.h"

#include "AssetRegistry/AssetRegistryModule.h"

void GEARSEditorUtils::HandleCreateAssetRequest(const FGeneratedAssetData& Data, UClass* AssetClass)
{
	if (!AssetClass) return;
	const auto PackagePath = Data.GetPackagePath();
	const auto AssetPath = Data.GetAssetPath();
	
	const auto Package = CreatePackage(*PackagePath);
	const auto NewAsset = NewObject<UObject>(Package, AssetClass, *Data.Name, RF_Public | RF_Standalone);
	FAssetRegistryModule::AssetCreated(NewAsset);
	if (!NewAsset->MarkPackageDirty()) UE_LOG(LogTemp, Warning, TEXT("GEARS_Editor: Asset %s failed to mark dirty."), *AssetPath);
	NewAsset->PreEditChange(nullptr);
	NewAsset->PostEditChange();
	Data.Asset = NewAsset;
	UE_LOG(LogTemp, Log, TEXT("GEARS_Editor: Asset %s created successfully."), *AssetPath);
}
