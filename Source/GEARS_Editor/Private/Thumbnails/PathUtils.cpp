#include "PathUtils.h"

#include "Settings/ThumbnailSaverSettings.h"

FString ThumbnailToTexture::PathUtils::GetTextureSavePathFor(const UObject* Asset)
{
	auto TextureName = FormatTextureName(Asset->GetName());
	FString PackageName = Asset->GetPackage()->GetName(); 
	FString DirectoryPath = FPaths::GetPath(PackageName); 
	return FPaths::Combine(DirectoryPath, TextureName);
}

FString ThumbnailToTexture::PathUtils::GetTextureSavePathFor(const UObject* Asset, const FThumbnailGenerationRule& Rule)
{
	const auto FullPath = GetTextureSavePathFor(Asset);
	auto BaseDirectory = FPaths::GetPath(FullPath);
	BaseDirectory.RightChopInline(Rule.SourcePath.Path.Len(), EAllowShrinking::No);
	const auto StartPath = FPaths::Combine(UThumbnailSaverSettings::GetRef().BasePath.Path, Rule.SubFolderDest);
	BaseDirectory.InsertAt(0, StartPath);
	const auto FileName = FPaths::GetBaseFilename(FullPath);
	return FPaths::Combine(BaseDirectory, FileName);
}

FString ThumbnailToTexture::PathUtils::FormatTextureName(const FString& InName)
{
	FString SourcePrefix, SourceName;
	auto& Settings = UThumbnailSaverSettings::GetRef();
	if (Settings.bAutoRemoveUpperPrefixInSource)
	{
		InName.Split("_", &SourcePrefix, &SourceName);
		if (SourcePrefix.ToUpper() != SourcePrefix) SourceName = InName;
	} else SourceName = InName;
	return Settings.OutputPrefix + SourceName;
}
