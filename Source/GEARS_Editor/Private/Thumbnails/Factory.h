#pragma once

namespace ThumbnailToTexture
{
	struct FAutoGenData;
}

namespace ThumbnailToTexture
{
	namespace Factory
	{
		FORCEINLINE FName MakeAssetName(const UPackage* Package)
		{
			if (Package) return *FPackageName::GetShortName(Package->GetName());
			return NAME_None;
		}
		
		UTexture2D* MakeTextureFromExistingThumbnail(const FAssetData& AssetData, UPackage* Package);
		
		UTexture2D* MakeTextureByRenderThumbnail(const FAutoGenData& AutoGenData);
		
		UTexture2D* MakeTextureFrom(const FObjectThumbnail& Thumbnail, UPackage* Package);
		
		UTexture2D* MakeTextureFrom(const FImage& Image, UPackage* Package, const FName& AssetName);
		FORCEINLINE UTexture2D* MakeTextureFrom(const FImage& Image, UPackage* Package)
		{
			return MakeTextureFrom(Image, Package, MakeAssetName(Package));
		}
		
		UTexture2D* MakeTextureFrom(const FImage& Image, int32 Width, int32 Height, UPackage* Package, const FName& AssetName);
		FORCEINLINE UTexture2D* MakeTextureFrom(const FImage& Image, const int32 Width, const int32 Height, UPackage* Package)
		{
			return MakeTextureFrom(Image, Width, Height, Package, MakeAssetName(Package));
		}
		
		void SaveTexture(UTexture2D* Texture);
	}
}

