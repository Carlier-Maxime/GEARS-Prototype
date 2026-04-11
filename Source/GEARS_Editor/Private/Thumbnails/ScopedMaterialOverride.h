#pragma once

namespace ThumbnailToTexture
{
	class FScopedMaterialOverride
	{
	public:
		FScopedMaterialOverride(UStaticMesh& InMesh, const TArrayView<UMaterialInterface*>& Overrides);
		~FScopedMaterialOverride();
	
	private:
		UStaticMesh& Mesh;
		TArray<UMaterialInterface*, TInlineAllocator<8>> OriginalMaterials;
	};
}
