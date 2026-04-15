#pragma once

namespace ThumbnailToTexture
{
	class FScopedMaterialOverride
	{
	public:
		FScopedMaterialOverride(UStaticMesh& InMesh, const TArrayView<UMaterialInterface*>& Overrides);
		FScopedMaterialOverride(UStaticMesh& InMesh, UMaterialInterface* Override);
		~FScopedMaterialOverride();
	
	private:
		UStaticMesh& Mesh;
		TArray<UMaterialInterface*, TInlineAllocator<8>> OriginalMaterials;
		bool bMeshHasDirty = false;
	};
}
