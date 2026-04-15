#include "Factory.h"

#include "AutoGenData.h"
#include "ObjectTools.h"
#include "ThumbnailRendererSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Settings/ThumbnailSaverSettings.h"
#include "UObject/SavePackage.h"

UTexture2D* ThumbnailToTexture::Factory::MakeTextureFromExistingThumbnail(const FAssetData& AssetData,
	UPackage* Package)
{
	FObjectThumbnail Thumbnail;
	if (!ThumbnailTools::LoadThumbnailFromPackage(AssetData, Thumbnail))
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to load thumbnail for %s."), *AssetData.GetFullName());
		return nullptr;
	}
	auto& Renderer = UThumbnailRendererSubsystem::GetRef();
	TArray64<uint8> Mask;
	Renderer.RenderMask(AssetData.GetAsset(), Mask);
	FImage Image;
	Thumbnail.GetImage().CopyTo(Image);
	FImage ImageResized;
	const auto Res = Renderer.GetRes();
	Image.ResizeTo(ImageResized, Res, Res, ERawImageFormat::BGRA8, Image.GammaSpace);
	Renderer.ApplyMask(ImageResized.RawData, Mask);
	return MakeTextureFrom(ImageResized, Package);
}

UTexture2D* ThumbnailToTexture::Factory::MakeTextureByRenderThumbnail(const FAutoGenData& AutoGenData)
{
	auto* Package = CreatePackage(*AutoGenData.SavePath);
	if (!AutoGenData.MaterialsOverrides || AutoGenData.MaterialsOverrides->IsEmpty())
	{
		return MakeTextureFromExistingThumbnail(AutoGenData.AssetData, Package);
	}
	
	auto* Mesh = Cast<UStaticMesh>(AutoGenData.AssetData.GetAsset());
	TArray<UMaterialInterface*, TInlineAllocator<8>> MaterialInterfaces;
	AutoGenData.GetLoadedMaterials(MaterialInterfaces);
	
	FObjectThumbnail Thumbnail;
	UThumbnailRendererSubsystem::GetRef().RenderThumbnail(Thumbnail, Mesh, MaterialInterfaces);
	if (!Thumbnail.HasValidImageData())
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to get thumbnail for %s."), *AutoGenData.AssetData.GetFullName());
		return nullptr;
	}
	
	return MakeTextureFrom(Thumbnail, CreatePackage(*AutoGenData.SavePath));
}

UTexture2D* ThumbnailToTexture::Factory::MakeTextureFrom(const FObjectThumbnail& Thumbnail, UPackage* Package)
{
	auto& Settings = UThumbnailSaverSettings::GetRef();
	const auto Width  = FMath::Min(Thumbnail.GetImageWidth(), Settings.MaxThumbnailSize);
	const auto Height = FMath::Min(Thumbnail.GetImageHeight(), Settings.MaxThumbnailSize);
	FImage Image;
	Thumbnail.GetImage().CopyTo(Image);
	return MakeTextureFrom(Image, Width, Height, Package);
}

UTexture2D* ThumbnailToTexture::Factory::MakeTextureFrom(const FImage& Image, UPackage* Package, const FName& AssetName)
{
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Make Texture Failed due to Package is null."));
		return nullptr;
	}
	Package->FullyLoad();
	
	if (Image.Format != ERawImageFormat::BGRA8)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Make Texture Failed due to Image format is not BGRA8."));
		return nullptr;
	}
	
	auto* Texture = NewObject<UTexture2D>(Package, AssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	if (!Texture)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to create texture for %s."), *Package->GetName());
		return nullptr;
	}

	Texture->Source.Init(Image.GetWidth(), Image.GetHeight(), 1, 1, TSF_BGRA8, Image.RawData.GetData());
	Texture->CompressionSettings = TC_BC7;
	Texture->MipGenSettings      = TMGS_NoMipmaps;
	Texture->LODGroup            = TEXTUREGROUP_UI;
	Texture->SRGB                = true;
	Texture->UpdateResource();

	FAssetRegistryModule::AssetCreated(Texture);
	if (!Package->MarkPackageDirty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ThumbnailToTexture: cannot be marked package dirty ! (%s)"), *Package->GetName())
	}

	return Texture;
}

UTexture2D* ThumbnailToTexture::Factory::MakeTextureFrom(const FImage& Image, const int32 Width, const int32 Height,
	UPackage* Package, const FName& AssetName)
{
	if (Image.SizeX == Width && Image.SizeY == Height) return MakeTextureFrom(Image, Package, AssetName);
	FImage DestImage(Width, Height, ERawImageFormat::BGRA8);
	Image.ResizeTo(DestImage, Width, Height, DestImage.Format, Image.GammaSpace);
	return MakeTextureFrom(DestImage, Package, AssetName);
}

void ThumbnailToTexture::Factory::SaveTexture(UTexture2D* Texture)
{
	const FString PackageFilename =
		FPackageName::LongPackageNameToFilename(
			Texture->GetPackage()->GetName(),
			FPackageName::GetAssetPackageExtension()
		);

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.Error         = GError;
	SaveArgs.bForceByteSwapping = false;
	SaveArgs.bWarnOfLongFilename = true;
	SaveArgs.SaveFlags = SAVE_NoError;

	UPackage::SavePackage(Texture->GetPackage(), Texture, *PackageFilename, SaveArgs);
}
