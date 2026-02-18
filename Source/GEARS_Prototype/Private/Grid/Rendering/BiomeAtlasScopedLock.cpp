#include "BiomeAtlasScopedLock.h"

#include "Data/BiomeType.h"
#include "Engine/Texture2DArray.h"
#include "Settings/GridParams.h"

FBiomeAtlasScopedLock::FBiomeAtlasScopedLock(UTexture2DArray& InAtlas, const int64 InPixelsPerSlice, const int64 InOctetsPerSlice) : 
		Atlas(InAtlas),
		PixelsPerSlice(InPixelsPerSlice),
		OctetsPerSlice(InOctetsPerSlice),
		RawData(Atlas.Source.IsValid() ? Atlas.Source.LockMip(0) : nullptr)
{}

FBiomeAtlasScopedLock::~FBiomeAtlasScopedLock()
{
	Atlas.Source.UnlockMip(0);
	Atlas.UpdateResource();
}

void FBiomeAtlasScopedLock::UpdateSliceFromBiomeTypes(const int32 SliceIndex, const TArray<int8>& BiomeTypes) const
{
	if (!RawData) return;
	auto* SliceData = RawData + SliceIndex * OctetsPerSlice;
	const auto& BiomeRegistry = GridParams::Get().GetBiomeRegistry();
	for (int32 i = 0; i < PixelsPerSlice; ++i)
	{
		const auto Color = BiomeTypes.IsValidIndex(i) && BiomeTypes[i] != -1 ? BiomeRegistry[BiomeTypes[i]]->Color.ToFColorSRGB() : FColor::Black;
		const auto PixelOffset = i * 4;
		SliceData[PixelOffset + 0] = Color.B;
		SliceData[PixelOffset + 1] = Color.G;
		SliceData[PixelOffset + 2] = Color.R;
		SliceData[PixelOffset + 3] = Color.A;
	}
}
