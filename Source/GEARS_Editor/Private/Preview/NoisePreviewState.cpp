#include "NoisePreviewState.h"

#include "Grid/Generator/BaseGenerator.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/SamplingContext.h"
#include "Grid/Types/GridPosition.h"

FNoisePreviewState::FNoisePreviewState()
{
	Brush.SetResourceObject(Texture.Get());
}

void FNoisePreviewState::Update()
{
	if (!StructPtr) return;
	
	if (!Texture || Texture->GetSizeX() != Settings.Resolution)
	{
		Texture.Reset(UTexture2D::CreateTransient(Settings.Resolution, Settings.Resolution, PF_B8G8R8A8));
		Texture->Filter = TF_Nearest;
		Brush.SetResourceObject(Texture.Get());
	}

	auto MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	const FNoisePreviewGenerators Generators = {
		BaseGenerator(Settings.Seed),
		ResourceGenerator(Settings.Seed)
	};
	for (int32 y = 0; y < Settings.Resolution; y++)
	{
		for (int32 x = 0; x < Settings.Resolution; x++)
		{
			MipData[y * Settings.Resolution + x] = GenerateColorAtPos(Generators, FGridPosition::FromGridPos(x, y));
		}
	}

	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
	Texture->PostEditChange();
}

FColor FNoisePreviewState::GenerateColorAtPos(const FNoisePreviewGenerators& Generators, const FGridPosition& Pos) const
{
	if (StructName == FNoiseContext::StaticStruct()->GetFName())
	{
		const auto Ctx = static_cast<FNoiseContext*>(StructPtr);
		const uint8 ColorVal = FMath::Floor(Generators.Base.GetNoiseDensity(Pos, *Ctx, FVector2D::ZeroVector) * 255.f);
		return {ColorVal, ColorVal, ColorVal, 255};
	}
	if (StructName == FSamplingContext::StaticStruct()->GetFName())
	{
		const auto Ctx = static_cast<FSamplingContext*>(StructPtr);
		const uint8 ColorVal = Generators.Resource.ShouldSpawn(Pos, *Ctx, FVector2D::ZeroVector) ? 255 : 0;
		return {ColorVal, ColorVal, ColorVal, 255};
	}
	return FColor::Black;
}
