// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Definitions/GEARS_Delegates.h"
#include "CameraSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class GEARS_PROTOTYPE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	float DefaultPitchAngle = -60;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	bool bInvertPitchAxis = false;
	FOnBoolChanged OnLockPitchChanged;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	float DefaultYawAngle = 0;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	bool bInvertYawAxis = false;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	float SnapYawDuration = 0.5f;
	FOnBoolChanged OnSnapYawStateChanged;
	
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
	
	float GetMinFOV() const;
	float GetMaxFOV() const;
	float GetFOVSpeed(float FOV) const;
	float GetDefaultFOV() const;
	
	float GetMinZoomDistance() const;
	float GetMaxZoomDistance() const;
	float GetDefaultZoomDistance() const;
	float GetZoomSpeed(float Distance) const;
	static float GetZoomDistance(float Factor);
	
	float GetMinPitch() const;
	float GetMaxPitch() const;
	float GetPitchSpeed(float Pitch) const;
	bool IsLockPitch() const;
	void SetLockPitch(bool bActive);
	
	float GetYawSpeed(float Yaw) const;
	bool IsSnapYaw90() const;
	void SetSnapYaw90(bool bActive);
private:
	TTuple<float, float> GetTimeRange(const TSoftObjectPtr<UCurveFloat>& Curve) const;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	TSoftObjectPtr<UCurveFloat> PitchCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Pitch")
	bool bLockPitch = false;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	TSoftObjectPtr<UCurveFloat> YawCurve;
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "Rotation|Yaw")
	bool bSnapYaw90 = false;
	
	UPROPERTY(Config, EditDefaultsOnly, config, Category = "FOV")
	TSoftObjectPtr<UCurveFloat> FOVCurve;
};
