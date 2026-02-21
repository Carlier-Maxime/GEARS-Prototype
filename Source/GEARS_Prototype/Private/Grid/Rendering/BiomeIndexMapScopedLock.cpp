#include "BiomeIndexMapScopedLock.h"

#include "Engine/Texture2DArray.h"
#include "Grid/Types/BiomeChunkIndex.h"
#include "Grid/Types/InBiomeChunkPos.h"

FBiomeIndexMapScopedLock::FBiomeIndexMapScopedLock(UTexture2DArray& InIndexMap)
	: FTextureScopedLock(InIndexMap, 1)
{}

size_t FBiomeIndexMapScopedLock::GetSliceIndex(const FBiomeChunkIndex& Index)
{
	const auto Diameter = GridMath::GetBiomeChunkMapDiameter();
	const auto Radius = GridMath::GetBiomeChunkMapRadius();
	return (Index.X + Radius) + (Index.Y + Radius) * Diameter;
}

void FBiomeIndexMapScopedLock::UpdateBiomeChunkPart(const int64 SliceIndex, const FInBiomeChunkPos& Pos,
	const TArray<uint8>& Biomes) const
{
	if (!RawData || SliceIndex >= Texture.GetPlatformData()->GetNumSlices()) return;
	auto* StartData = RawData + SliceIndex * OctetsPerSlice;
	StartData += Pos.Y * GridParams::Get().GetChunkSize() * Texture.GetPlatformData()->SizeX * OctetsPerPixel;
	StartData += Pos.X * GridParams::Get().GetChunkSize() * OctetsPerPixel;
	for (auto Local : FChunkIndex{})
	{
		auto* LocalData = StartData + Local.Y * Texture.GetPlatformData()->SizeX * OctetsPerPixel;
		LocalData += Local.X * OctetsPerPixel;
		LocalData[0] = Biomes[Local.Flatten()];
	}
}
