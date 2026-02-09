// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "SamplingContext.generated.h"

/**
 * 
 */
USTRUCT()
struct FSamplingContext
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Sampling|Transform", meta = (ClampMin = 0, ClampMax = 0.3))
	float JitterMaxOffset = 0.05f;
	
	UPROPERTY(EditAnywhere, Category = "Sampling|Transform")
	TObjectPtr<UCurveFloat> ScaleDistributionCurve = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Sampling|Transform")
	bool bRandomYaw = false;
	
	UPROPERTY(EditAnywhere, Category = "Sampling|Noise", meta = (ClampMin = 0, ClampMax = 1))
	FNoiseContext Noise;
	
	UPROPERTY(EditAnywhere, Category = "Sampling|Noise", meta = (ClampMin = 0, ClampMax = 1))
	float NoiseThreshold = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Sampling|Noise", meta = (ClampMin = 0, ClampMax = 1))
	float ThresholdSmoothing = 0; 
};
