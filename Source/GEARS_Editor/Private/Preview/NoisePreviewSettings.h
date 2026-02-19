#pragma once

#include "CoreMinimal.h"
#include "NoisePreviewSettings.generated.h"

USTRUCT()
struct FNoisePreviewSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(UIMin=0, UIMax=1000))
	int32 Seed = 0;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=32, UIMax=512, ClampMax=2048, Delta=32))
	int32 Resolution = 256;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=64, ClampMax=256, Delta=32))
	int32 ThumbnailSize = 128;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=1, UIMax=16))
	int32 SamplingStep = 1;
};