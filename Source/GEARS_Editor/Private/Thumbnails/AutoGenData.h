#pragma once

namespace ThumbnailToTexture
{
	struct FAutoGenData
	{
		FAssetData AssetData;
		FString SavePath;
		const TArray<TSoftObjectPtr<UMaterialInstance>>* MaterialsOverrides = nullptr;
		FDateTime LastGenTime = FDateTime::MinValue();
		
		[[nodiscard]] bool NeedGenerate() const;
	};
}
