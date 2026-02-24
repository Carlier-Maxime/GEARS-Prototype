// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "DistributionRule.generated.h"

/**
 * 
 */
USTRUCT()
struct FDistributionRule
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FNoiseContext Noise;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 1))
	float NoiseThreshold = 0.6f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 1))
	float ThresholdSmoothing = 0; 
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, UIMax = 16))
	uint32 Spacing = 0;
};
