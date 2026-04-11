#include "ThumbnailRendererSubsystem.h"

#include "ObjectTools.h"
#include "ScopedMaterialOverride.h"
#include "Settings/ThumbnailSaverSettings.h"

void UThumbnailRendererSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Res = UThumbnailSaverSettings::GetRef().MaxThumbnailSize;
	SharedRT = NewObject<UTextureRenderTarget2D>(this);
	SharedRT = NewObject<UTextureRenderTarget2D>(
		GetTransientPackage(), 
		UTextureRenderTarget2D::StaticClass(),
		NAME_None,
		RF_Transient
	);
	SharedRT->ClearColor = FLinearColor(0.f, 0.f, 0.f, 0.f);
	SharedRT->TargetGamma = 2.2f;
	SharedRT->InitCustomFormat(Res, Res, PF_B8G8R8A8, true);
	SharedRT->UpdateResourceImmediate(true);
}

void UThumbnailRendererSubsystem::Deinitialize()
{
	if(SharedRT) SharedRT->ReleaseResource();
	SharedRT = nullptr;
}

UThumbnailRendererSubsystem& UThumbnailRendererSubsystem::GetRef()
{
	return *GEditor->GetEditorSubsystem<UThumbnailRendererSubsystem>();
}

void UThumbnailRendererSubsystem::DummyRender(UObject& InObject)
{
	FObjectThumbnail DummyThumbnail;
	ThumbnailTools::RenderThumbnail(
		&InObject, Res, Res,
		ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush,
		SharedRT->GameThread_GetRenderTargetResource(),
		&DummyThumbnail
	);
	FlushRenderingCommands();
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
	ThumbnailToTexture::FScopedMaterialOverride MatOverride(*Mesh, MaterialOverrides);
	
	auto* RenderResource = SharedRT->GameThread_GetRenderTargetResource();
	if (!RenderResource) UE_LOG(LogTemp, Error, TEXT("ThumbnailToTexture: Failed to get render target resource."));
	FlushRenderingCommands();
	
	ThumbnailTools::RenderThumbnail(
		Mesh,
		Res,
		Res,
		ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush,
		RenderResource,
		&OutThumbnail
	);
}
