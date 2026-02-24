#include "BiomeTextureMapper.h"

#include "Engine/Texture2DArray.h"
#include "Grid/GridMath.h"
#include "Settings/GridParams.h"

FBiomeTextureMapper::FBiomeTextureMapper() : FBiomeTextureMapper(*GridParams::Get().GetBiomeAtlas(), *GridParams::Get().GetBiomeIndexMap()) {}

FBiomeTextureMapper::FBiomeTextureMapper(UTexture2DArray& InAtlas, UTexture2DArray& InIndexMap) : Atlas(InAtlas), IndexMap(InIndexMap)
{
	const auto& Params = GridParams::Get();
	ConfigureT2A(Atlas, 1, Params.GetBiomeRegistry().Num()+1, PF_B8G8R8A8);
	ConfigureT2A(IndexMap, Params.GetBiomeChunkFactor() << Params.GetChunkShift(), GridMath::GetBiomeChunkCount(), PF_R8);
	FillAtlas();
}

void FBiomeTextureMapper::ConfigureT2A(UTexture2DArray& Texture, const uint16 Size, const uint16 Depth, const EPixelFormat& Format)
{
	const bool bIsDataFormat = Format == PF_R8|| Format == PF_R16_UINT || Format == PF_R32_UINT;
	
	auto* PlatformData = new FTexturePlatformData();
	PlatformData->SizeX = Size;
	PlatformData->SizeY = Size;
	PlatformData->SetNumSlices(Depth);
	PlatformData->PixelFormat = Format;
	
	auto* Mip = new FTexture2DMipMap();
	PlatformData->Mips.Add(Mip);
	Mip->SizeX = Size;
	Mip->SizeY = Size;
	Mip->SizeZ = Depth;

	const int64 BlockBytes = GPixelFormats[Format].BlockBytes;
	const int64 TotalBytes = static_cast<int64>(Size) * Size * Depth * BlockBytes;

	Mip->BulkData.Lock(LOCK_READ_WRITE);
	Mip->BulkData.Realloc(TotalBytes);
	Mip->BulkData.Unlock();
	
	Texture.ReleaseResource();
	Texture.SetPlatformData(PlatformData);
	Texture.SRGB = !bIsDataFormat;
	Texture.NeverStream = true;
	Texture.CompressionSettings = bIsDataFormat ? TC_Displacementmap : TC_Default;
	Texture.Filter = TF_Nearest;
	Texture.UpdateResource();
}

void FBiomeTextureMapper::FillAtlas()
{
	const FTextureScopedLock Texture(Atlas, 4);
	const auto& Registry = GridParams::Get().GetBiomeRegistry();
	FColor Color;
	auto Lambda_SetColor = [&](FPixelWriteContext Ctx)
	{
		Ctx.SliceData[Ctx.PixelOffset] = Color.B;
		Ctx.SliceData[Ctx.PixelOffset + 1] = Color.G;
		Ctx.SliceData[Ctx.PixelOffset + 2] = Color.R;
		Ctx.SliceData[Ctx.PixelOffset + 3] = Color.A;
	};
	for (int32 i=0; i<Registry.Num(); ++i)
	{
		Color = Registry[i].Color.ToFColorSRGB();
		Texture.UpdateSlice(i, Lambda_SetColor);
	}
	Color = FColor::Black;
	Texture.UpdateSlice(Registry.Num(), Lambda_SetColor);
}

FBiomeIndexMapScopedLock FBiomeTextureMapper::Lock() const
{

	return FBiomeIndexMapScopedLock{IndexMap};
}
