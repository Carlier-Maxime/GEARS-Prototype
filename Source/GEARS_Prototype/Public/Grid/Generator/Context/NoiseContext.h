#pragma once

#include "CoreMinimal.h"
#include "NoiseContext.generated.h"

USTRUCT()
struct FNoiseContext
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Noise")
	float Frequency = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Noise", meta = (ClampMin = 1, ClampMax = 8))
	int32 Octaves = 3;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float Persistence = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Noise")
	float Lacunarity = 2.0f;
};
