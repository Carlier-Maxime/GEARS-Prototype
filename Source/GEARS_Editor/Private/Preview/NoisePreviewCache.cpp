#include "NoisePreviewCache.h"

#include "Grid/Generator/BaseGenerator.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/SamplingContext.h"
#include "Grid/Types/GridPosition.h"

FNoisePreviewCache::FNoisePreviewCache()
{
	Brush.SetResourceObject(Texture.Get());
}

void FNoisePreviewCache::Update()
{
	if (!StructPtr) return;
	
	if (!Texture || Texture->GetSizeX() != Resolution)
	{
		Texture.Reset(UTexture2D::CreateTransient(Resolution, Resolution, PF_B8G8R8A8));
		Texture->Filter = TF_Nearest;
		Brush.SetResourceObject(Texture.Get());
	}

	auto MipData = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	const auto Generator = BaseGenerator(Seed);
	for (int32 y = 0; y < Resolution; y++)
	{
		for (int32 x = 0; x < Resolution; x++)
		{
			MipData[y * Resolution + x] = GenerateColorAtPos(Generator, FGridPosition::FromGridPos(x, y));
		}
	}

	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
	Texture->PostEditChange();
}

FColor FNoisePreviewCache::GenerateColorAtPos(const BaseGenerator& Generator, const FGridPosition& Pos) const
{
	if (StructName == FNoiseContext::StaticStruct()->GetFName())
	{
		const auto Ctx = static_cast<FNoiseContext*>(StructPtr);
		const uint8 ColorVal = FMath::Floor(Generator.GetNoiseDensityAtPosition(Pos, *Ctx) * 255.f);
		return {ColorVal, ColorVal, ColorVal, 255};
	}
	if (StructName == FSamplingContext::StaticStruct()->GetFName())
	{
		const auto Ctx = static_cast<FSamplingContext*>(StructPtr);
		const uint8 ColorVal = Generator.ShouldSpawnResource(Pos, *Ctx) ? 255 : 0;
		return {ColorVal, ColorVal, ColorVal, 255};
	}
	return FColor::Black;
}
