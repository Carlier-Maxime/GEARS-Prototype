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
	const auto Worker = [this, Res, HalfRes, Step](int32 Index)
	{
		const int32 x = Index % Res;
		const int32 y = Index / Res;
		const auto WorldX = (x - HalfRes) * Step;
		const auto WorldY = (y - HalfRes) * Step;
		PixelBuffer[Index] = OnGenerateColor.IsBound() ? OnGenerateColor.Execute(FWorldGridPos(WorldX, WorldY)) : FColor::Black;
	};
	const auto Size = Res * Res;
	if (Res < 64) for (auto I = 0; I < Size; ++I) Worker(I);
	else ParallelFor(Size, Worker);
	
	auto* Region = new FUpdateTextureRegion2D(0, 0, 0, 0, Res, Res);
	Texture->UpdateTextureRegions(0, 1, Region, Res * 4, 4, reinterpret_cast<uint8*>(PixelBuffer.GetData()), 
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
			delete Regions;
		});
}
