// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	float NoiseThreshold = 0.6f;
	
	UPROPERTY(EditAnywhere, Category = "Sampling|Noise", meta = (ClampMin = 0, ClampMax = 1))
	float ThresholdSmoothing = 0;

	UPROPERTY(EditAnywhere, Category = "Sampling|Noise", meta = (ClampMin = 0, ClampMax = 1))
	float NoiseScale = 0.05f;
	
	FVector2D Offset = FVector2D::ZeroVector;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Sampling|Noise|Preview")
	TObjectPtr<UTexture2D> NoisePreview;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling|Noise|Preview", meta=(ClampMin = 8))
	int32 PreviewResolution = 256;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling|Noise|Preview", meta=(UIMin = 0, UIMax = 10000))
	int32 SeedPreview = 0;
	
	void UpdatePreview();
#endif
};
