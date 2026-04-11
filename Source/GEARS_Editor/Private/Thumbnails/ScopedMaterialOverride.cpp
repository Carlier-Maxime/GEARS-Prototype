#include "ScopedMaterialOverride.h"

using namespace ThumbnailToTexture;

FScopedMaterialOverride::FScopedMaterialOverride(UStaticMesh& InMesh, const TArrayView<UMaterialInterface*>& Overrides) : Mesh(InMesh)
{
	const int32 Num = Mesh.GetStaticMaterials().Num();
	OriginalMaterials.Reserve(Num);

	for (int32 i = 0; i < Num; ++i)
	{
		OriginalMaterials.Add(Mesh.GetMaterial(i));
		if (!Overrides.IsValidIndex(i) || !Overrides[i]) continue;
		Mesh.SetMaterial(i, Overrides[i]);
		Mesh.GetMaterial(i)->EnsureIsComplete();
	}
}

FScopedMaterialOverride::~FScopedMaterialOverride()
{
	for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
	{
		Mesh.SetMaterial(i, OriginalMaterials[i]);
	}
}
