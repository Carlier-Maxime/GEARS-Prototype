#pragma once

#include "Engine/TextureRenderTarget2D.h"
#include "ThumbnailRendererSubsystem.generated.h"

UCLASS()
class UThumbnailRendererSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTextureRenderTarget2D* SharedRT;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static UThumbnailRendererSubsystem& GetRef();
	
	void RenderThumbnail(FObjectThumbnail& OutThumbnail, UStaticMesh* Mesh, const TArrayView<UMaterialInterface*>& MaterialOverrides);
	
private:
	void DummyRender(UObject& InObject);
	bool NeedDummy = true;
	int32 Res;
};