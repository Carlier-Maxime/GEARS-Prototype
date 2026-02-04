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
	virtual void PostInitProperties() override;

	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	float DefaultPitchAngle = -60;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	bool bInvertPitchAxis = false;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	TSoftObjectPtr<UCurveFloat> PitchCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	bool bLockPitch = false;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	float DefaultYawAngle = 0;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	bool bInvertYawAxis = false;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	float SnapYawDuration = 0.5f;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	TSoftObjectPtr<UCurveFloat> YawCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	bool bSnapYaw90 = false;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta = (
		ToolTip="Multiplier based on the grid cell size"))
	TSoftObjectPtr<UCurveFloat> ZoomFactorCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom", meta=(
		ToolTip="Multiplier based on the grid cell size."))
	float DefaultZoomDistanceFactor = 25;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Zoom")
	bool bInvertZoomAxis = false;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "FOV")
	float DefaultFOV = 70;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "FOV")
	TSoftObjectPtr<UCurveFloat> FOVCurve;
};
