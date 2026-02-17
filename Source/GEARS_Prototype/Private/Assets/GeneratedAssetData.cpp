#include "Assets/GeneratedAssetData.h"

FString FGeneratedAssetData::GetPackagePath() const
{
	return FPaths::Combine(GeneratedFolderStr, SubFolder, Name);
}

FString FGeneratedAssetData::GetAssetPath(const FString& PackagePath) const
{
	return FString::Printf(TEXT("%s.%s"), *PackagePath, *Name);
}

FString FGeneratedAssetData::GetAssetPath() const
{
	return GetAssetPath(GetPackagePath());
}
