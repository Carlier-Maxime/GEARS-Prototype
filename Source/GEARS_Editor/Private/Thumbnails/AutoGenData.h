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
		
		template <typename AllocatorType>
		void GetLoadedMaterials(TArray<UMaterialInterface*, AllocatorType>& OutMaterials) const
		{
			OutMaterials.Reset();
			OutMaterials.Reserve(MaterialsOverrides->Num());
			for (const auto& MaterialOverride : *MaterialsOverrides)
			{
				OutMaterials.Emplace(MaterialOverride.LoadSynchronous());
			}
		}
	};
}
