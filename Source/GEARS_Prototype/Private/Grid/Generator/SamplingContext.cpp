// Fill out your copyright notice in the Description page of Project Settings.

#include "SamplingContext.h"

#include "BaseGenerator.h"

#if WITH_EDITORONLY_DATA
void FSamplingContext::UpdatePreview()
{
	if (!NoisePreview || NoisePreview->GetSizeX() != PreviewResolution)
	{
		NoisePreview = UTexture2D::CreateTransient(PreviewResolution, PreviewResolution, PF_B8G8R8A8);
		NoisePreview->Filter = TF_Nearest;
	}

	auto MipData = static_cast<FColor*>(NoisePreview->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	const auto Generator = BaseGenerator(SeedPreview, false);
	for (int32 y = 0; y < PreviewResolution; y++)
	{
		for (int32 x = 0; x < PreviewResolution; x++)
		{
			const uint8 ColorVal = Generator.ShouldSpawnResource(FGridPosition::FromGridPos(x, y), *this) ? 255 : 0;
			const int32 Index = (y * PreviewResolution + x);
			MipData[Index] = FColor(ColorVal, ColorVal, ColorVal, 255);
		}
	}

	NoisePreview->GetPlatformData()->Mips[0].BulkData.Unlock();
	NoisePreview->UpdateResource();
	NoisePreview->PostEditChange(); 
}
#endif