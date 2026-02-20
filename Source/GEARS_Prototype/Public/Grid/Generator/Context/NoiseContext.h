#pragma once

#include "CoreMinimal.h"
#include "NoiseContext.generated.h"

USTRUCT()
struct FNoiseContext
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Noise", meta=(ClampMin = 0.00001, UIMax=0.5, ClampMax = 1))
	float Frequency = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Noise", meta=(ClampMin = 1, ClampMax = 8))
	int32 Octaves = 3;

	UPROPERTY(EditAnywhere, Category = "Noise", meta=(ClampMin = 0, ClampMax = 1))
	float Persistence = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Noise", meta=(ClampMin = 1, UIMax = 4))
	float Lacunarity = 2.0f;
};
