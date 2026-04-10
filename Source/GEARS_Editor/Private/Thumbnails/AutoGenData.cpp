#include "AutoGenData.h"

using namespace ThumbnailToTexture;

bool FAutoGenData::NeedGenerate() const
{
	FString FilePath;
	if (!FPackageName::DoesPackageExist(SavePath, &FilePath)) return true;
	const auto TextureTime = IFileManager::Get().GetTimeStamp(*FilePath);
	if (!FPackageName::DoesPackageExist(AssetData.PackageName.ToString(), &FilePath)) return false;
	const auto AssetTime = IFileManager::Get().GetTimeStamp(*FilePath);
	return AssetTime > (TextureTime > LastGenTime ? TextureTime : LastGenTime);
}
