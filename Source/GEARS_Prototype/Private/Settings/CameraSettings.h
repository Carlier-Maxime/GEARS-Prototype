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
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation", meta=(
		ClampMin = -0.01, ClampMax = -90))
	float MinPitchAngle = -30;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation", meta=(
		ClampMin = -0.01, ClampMax = -90))
	float MaxPitchAngle = -70;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation", meta=(
		ClampMin = -0.01, ClampMax = -90))
	float DefaultPitchAngle = -50;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation", meta = (
		UIMin = 0.1, UIMax = 10.0, 
		ClampMin = 0.01))
	float RotationSpeed = 1;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation")
	bool bInvertRotationAxis = false;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta=(
		ClampMin = 0.1, ClampMax = 100, ToolTip="Multiplier based on the grid cell size."))
	float MinZoomDistanceFactor = 10;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta=(
		ClampMin = 10, ClampMax = 1000, ToolTip="Multiplier based on the grid cell size."))
	float MaxZoomDistanceFactor = 100;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta=(
		ClampMin = 0.1, ClampMax = 1000, ToolTip="Multiplier based on the grid cell size."))
	float DefaultZoomDistanceFactor = 50;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta = (
		UIMin = 0.1, UIMax = 10.0, 
		ClampMin = 0.01))
	float ZoomSpeed = 1;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom")
	bool bInvertZoomAxis = false;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetMinZoomDistance() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetMaxZoomDistance() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	float GetDefaultZoomDistance() const;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	static float GetZoomDistance(float Factor);
};
