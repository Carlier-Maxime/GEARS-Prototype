#pragma once
#include "AutoGenData.h"

class UThumbnailSaverSettings;

namespace ThumbnailToTexture
{
	class GEARS_EDITOR_API FManager
	{
	public:
		static FManager& Get();
		
		void PrepareAutoThumbnails(UThumbnailSaverSettings* Settings);
		void AutoGenerateThumbnails(bool ForceGen = false);
	
	private:
		explicit FManager();
		~FManager();
		
		void InitializePlaceholderTextures();
		
		int32 Res;
		const FImage PlaceHolder;
		TArray<FAutoGenData> AutoGenFromAssets;
	};

}
