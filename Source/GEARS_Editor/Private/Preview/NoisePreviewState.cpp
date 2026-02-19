#include "NoisePreviewState.h"

#include "Grid/Types.h"

FNoisePreviewState::FNoisePreviewState()
{
	Brush.SetResourceObject(Texture.Get());
}

void FNoisePreviewState::Update(const FNoisePreviewContext& Ctx)
{
	const int32 Res = Ctx.Resolution;
	
	if (!Texture || Texture->GetSizeX() != Res)
	{
		Texture.Reset(UTexture2D::CreateTransient(Res, Res, PF_B8G8R8A8));
		Texture->Filter = TF_Nearest;
		Texture->UpdateResource();
		Brush.SetResourceObject(Texture.Get());
		PixelBuffer.SetNumUninitialized(Res * Res);
	}
	
	const auto Step = Ctx.SamplingStep;
	const auto HalfRes = Res >> 1;
	for (auto Iy = 0; Iy < Res; ++Iy)
	{
		const auto WorldY = (Iy - HalfRes) * Step;
		const auto RowOffset = Iy * Res;
		for (auto Ix = 0; Ix < Res; ++Ix)
		{
			const auto WorldX = (Ix - HalfRes) * Step;
			PixelBuffer[RowOffset + Ix] = OnGenerateColor.IsBound() ? OnGenerateColor.Execute(FWorldGridPos(WorldX, WorldY)) : FColor::Black;
		}
	}
	
	auto* Region = new FUpdateTextureRegion2D(0, 0, 0, 0, Res, Res);
	Texture->UpdateTextureRegions(0, 1, Region, Res * 4, 4, reinterpret_cast<uint8*>(PixelBuffer.GetData()), 
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
			delete Regions;
		});
}
