#include "NoisePreviewState.h"

#include "Grid/Types/GridPosition.h"

FNoisePreviewState::FNoisePreviewState()
{
	Brush.SetResourceObject(Texture.Get());
}

void FNoisePreviewState::Update()
{
	const int32 Res = Settings.Resolution;
	
	if (!Texture || Texture->GetSizeX() != Res)
	{
		Texture.Reset(UTexture2D::CreateTransient(Res, Res, PF_B8G8R8A8));
		Texture->Filter = TF_Nearest;
		Texture->UpdateResource();
		Brush.SetResourceObject(Texture.Get());
		PixelBuffer.SetNumUninitialized(Res * Res);
	}
	
	for (int32 y = 0; y < Res; y++)
	{
		for (int32 x = 0; x < Res; x++)
		{
			PixelBuffer[y * Res + x] = OnGenerateColor.Execute(FGridPosition::FromGridPos(x, y));
		}
	}
	
	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D(0, 0, 0, 0, Res, Res);
	Texture->UpdateTextureRegions(0, 1, Region, Res * 4, 4, reinterpret_cast<uint8*>(PixelBuffer.GetData()), 
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
			delete Regions;
		});
}
