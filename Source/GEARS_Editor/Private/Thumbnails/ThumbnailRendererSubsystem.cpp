#include "ThumbnailRendererSubsystem.h"

#include "ObjectTools.h"
#include "ScopedMaterialOverride.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Settings/ThumbnailSaverSettings.h"

void UThumbnailRendererSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	const auto& Settings = UThumbnailSaverSettings::GetRef();
	Res = Settings.MaxThumbnailSize;
	RT_Mask = CreateRT(Res);
	RT_Color = CreateRT(Res);
}

UTextureRenderTarget2D* UThumbnailRendererSubsystem::CreateRT(const uint32 Res)
{
	auto* RT = NewObject<UTextureRenderTarget2D>(
		GetTransientPackage(), 
		UTextureRenderTarget2D::StaticClass(),
		NAME_None,
		RF_Transient
	);
	RT->ClearColor = FLinearColor(0.f, 0.f, 0.f, 0.f);
	RT->TargetGamma = 2.2f;
	RT->InitCustomFormat(Res, Res, PF_B8G8R8A8, true);
	RT->UpdateResourceImmediate(true);
	return RT;
}

void UThumbnailRendererSubsystem::Deinitialize()
{
	if(RT_Mask) RT_Mask->ReleaseResource();
	RT_Mask = nullptr;
	if(RT_Color) RT_Color->ReleaseResource();
	RT_Color = nullptr;
}

UThumbnailRendererSubsystem& UThumbnailRendererSubsystem::GetRef()
{
	return *GEditor->GetEditorSubsystem<UThumbnailRendererSubsystem>();
}

void UThumbnailRendererSubsystem::DummyRender(UObject& InObject) const
{
	FObjectThumbnail DummyThumbnail;
	ThumbnailTools::RenderThumbnail(
		&InObject, Res, Res,
		ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush,
		RT_Color->GameThread_GetRenderTargetResource(),
		&DummyThumbnail
	);
	NeedDummy = false;
}

void UThumbnailRendererSubsystem::RenderThumbnail(FObjectThumbnail& OutThumbnail, UStaticMesh* Mesh,
	const TArrayView<UMaterialInterface*>& MaterialOverrides)
{
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Asset %s is not a static mesh or is invalid."), *Mesh->GetFullName());
		return;
	}
	if (NeedDummy) DummyRender(*Mesh);
	TArray64<uint8> ColorData, MaskData;
	RenderMask(Mesh, MaskData);
	{
		ThumbnailToTexture::FScopedMaterialOverride MatOverride(*Mesh, MaterialOverrides);
		ThumbnailTools::RenderThumbnail(
			Mesh,
			Res,
			Res,
			ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush,
			RT_Color->GameThread_GetRenderTargetResource(),
			nullptr
		);
	}
	ColorData.SetNumUninitialized(Res * Res * 4);
	RT_Color->GameThread_GetRenderTargetResource()->ReadPixelsPtr(reinterpret_cast<FColor*>(ColorData.GetData()), FReadSurfaceDataFlags());
	ApplyMask(ColorData, MaskData);
	const FImageView MyImageView(ColorData.GetData(), Res, Res, ERawImageFormat::BGRA8);
	OutThumbnail.SetImageSize(Res, Res);
	OutThumbnail.SetImage(MyImageView);
}

void UThumbnailRendererSubsystem::RenderMask(UObject* InObject, TArray64<uint8>& OutPixels) const
{
	if (auto* Mesh = Cast<UStaticMesh>(InObject); Mesh) return RenderMask(Mesh, OutPixels);
	OutPixels.Reserve(Res * Res * 4);
	for (uint32 i = 0; i < Res * Res * 4; ++i) OutPixels.Add(255);
}

void UThumbnailRendererSubsystem::RenderMask(UStaticMesh* Mesh, TArray64<uint8>& OutPixels) const
{
	auto WhiteMat = UThumbnailSaverSettings::GetRef().WhiteMat;
	if (WhiteMat.IsNull()) UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: WhiteMat is not set."));
	ThumbnailToTexture::FScopedMaterialOverride MatOverride(*Mesh, WhiteMat.LoadSynchronous());
	if (NeedDummy) DummyRender(*Mesh);
	ThumbnailTools::RenderThumbnail(
		Mesh,
		Res,
		Res,
		ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush,
		RT_Mask->GameThread_GetRenderTargetResource(),
		nullptr
	);
	OutPixels.SetNumUninitialized(Res * Res * 4);
	RT_Mask->GameThread_GetRenderTargetResource()->ReadPixelsPtr(reinterpret_cast<FColor*>(OutPixels.GetData()), FReadSurfaceDataFlags());
}

void UThumbnailRendererSubsystem::ApplyMask(TArray64<uint8>& Pixels, const TArray64<uint8>& MaskPixels)
{
	check(Pixels.Num() == MaskPixels.Num() && Pixels.Num() == Res * Res * 4);
	for (uint32 i = 0; i < Res * Res; ++i)
	{
		Pixels[i * 4 + 3] = MaskPixels[i * 4] > 128 ? Pixels[i * 4 + 3] : 0;
	}
}
