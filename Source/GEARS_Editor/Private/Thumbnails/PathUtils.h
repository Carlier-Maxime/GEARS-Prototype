#pragma once

struct FThumbnailGenerationRule;

namespace ThumbnailToTexture
{
	namespace PathUtils
	{
		FString GetTextureSavePathFor(const UObject* Asset);
		FString GetTextureSavePathFor(const UObject* Asset, const FThumbnailGenerationRule& Rule);
		FString FormatTextureName(const FString& InName);
	}
}
