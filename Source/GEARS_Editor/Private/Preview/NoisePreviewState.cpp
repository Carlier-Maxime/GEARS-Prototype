#include "NoisePreviewState.h"

#include "Grid/Types.h"

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
	
	const auto DemiRes = Res >> 1;
	for (int32 y = -DemiRes; y < DemiRes; y++)
	{
		const int32 RowOffset = (y + DemiRes) * Res;
		for (int32 x = -DemiRes; x < DemiRes; x++)
		{
			const int32 PixelIndex = RowOffset + (x + DemiRes);
			PixelBuffer[PixelIndex] = OnGenerateColor.Execute(FWorldGridPos(x, y));
		}
	}
	
	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D(0, 0, 0, 0, Res, Res);
	Texture->UpdateTextureRegions(0, 1, Region, Res * 4, 4, reinterpret_cast<uint8*>(PixelBuffer.GetData()), 
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
			delete Regions;
		});
}
