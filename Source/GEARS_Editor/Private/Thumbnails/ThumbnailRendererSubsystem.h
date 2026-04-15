#pragma once

#include "ThumbnailRendererSubsystem.generated.h"

UCLASS()
class UThumbnailRendererSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTextureRenderTarget2D* RT_Mask;
	UPROPERTY()
	UTextureRenderTarget2D* RT_Color;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static UThumbnailRendererSubsystem& GetRef();
	
	void RenderThumbnail(FObjectThumbnail& OutThumbnail, UStaticMesh* Mesh, const TArrayView<UMaterialInterface*>& MaterialOverrides);
	void RenderMask(UObject* InObject, TArray64<uint8>& OutPixels) const;
	void RenderMask(UStaticMesh* Mesh, TArray64<uint8>& OutPixels) const;
	void ApplyMask(TArray64<uint8>& Pixels, const TArray64<uint8>& MaskPixels);
	FORCEINLINE uint32 GetRes() const {return Res;}
	
private:
	static UTextureRenderTarget2D* CreateRT(uint32 Res);
	void DummyRender(UObject& InObject) const;
	mutable bool NeedDummy = true;
	uint32 Res;
};