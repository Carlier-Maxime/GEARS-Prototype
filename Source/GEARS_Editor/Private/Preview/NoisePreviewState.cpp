#include "NoisePreviewState.h"

#include "Grid/Types/GridPosition.h"

FNoisePreviewState::FNoisePreviewState()
{
	Brush.SetResourceObject(Texture.Get());
}

void FNoisePreviewState::Update()
{
	if (!Texture || Texture->GetSizeX() != Settings.Resolution)
	{
		Texture.Reset(UTexture2D::CreateTransient(Settings.Resolution, Settings.Resolution, PF_B8G8R8A8));
		Texture->Filter = TF_Nearest;
		Brush.SetResourceObject(Texture.Get());
	}
	
	if (!OnGenerateColor.IsBound()) return;
	auto MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	for (int32 y = 0; y < Settings.Resolution; y++)
	{
		for (int32 x = 0; x < Settings.Resolution; x++)
		{
			MipData[y * Settings.Resolution + x] = OnGenerateColor.Execute(FGridPosition::FromGridPos(x, y));
		}
	}

	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
	Texture->PostEditChange();
}
