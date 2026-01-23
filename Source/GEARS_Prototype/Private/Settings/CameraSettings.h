// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class GEARS_PROTOTYPE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation")
	TSoftObjectPtr<UCurveFloat> PitchCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation")
	float DefaultPitchAngle = -60;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation")
	bool bInvertRotationAxis = false;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta = (
		ToolTip="Multiplier based on the grid cell size"))
	TSoftObjectPtr<UCurveFloat> ZoomFactorCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta=(
		ToolTip="Multiplier based on the grid cell size."))
	float DefaultZoomDistanceFactor = 25;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom")
	bool bInvertZoomAxis = false;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetMinZoomDistance() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetMaxZoomDistance() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetDefaultZoomDistance() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetZoomSpeed(float Distance) const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	static float GetZoomDistance(float Factor);
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetMinPitch() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetMaxPitch() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetPitchSpeed(float Pitch) const;
private:
	TTuple<float, float> GetTimeRange(const TSoftObjectPtr<UCurveFloat>& Curve) const;
};
