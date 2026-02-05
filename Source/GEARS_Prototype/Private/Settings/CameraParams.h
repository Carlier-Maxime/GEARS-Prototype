#pragma once
#include "Definitions/GEARS_Delegates.h"

class CameraParams
{
public:
	static CameraParams& Get()
	{
		static CameraParams Instance;
		return Instance;
	}
	
	float GetMinFOV() const;
	float GetMaxFOV() const;
	float GetFOVSpeed(float FOV) const;
	float GetDefaultFOV() const;
	
	float GetMinZoomDistance() const;
	float GetMaxZoomDistance() const;
	float GetDefaultZoomDistance() const;
	float GetZoomSpeed(float Distance) const;
	bool IsInvertZoomAxis() const;
	static float GetZoomDistance(float Factor);
	
	float GetMinPitch() const;
	float GetMaxPitch() const;
	float GetPitchSpeed(float Pitch) const;
	bool IsLockPitch() const;
	void SetLockPitch(bool bActive);
	
	float GetYawSpeed(float Yaw) const;
	bool IsSnapYaw90() const;
	void SetSnapYaw90(bool bActive);
	
	FOnBoolChanged OnLockPitchChanged;
	FOnBoolChanged OnSnapYawStateChanged;

private:
	CameraParams() = default;
	friend class UCameraSettings;
	
	TTuple<float, float> GetTimeRange(const TSoftObjectPtr<UCurveFloat>& Curve) const;
	
	TSoftObjectPtr<UCurveFloat> ZoomFactorCurve;
	float DefaultZoomDistanceFactor;
	bool bInvertZoomAxis;
	
	TSoftObjectPtr<UCurveFloat> PitchCurve;
	bool bLockPitch;
	float DefaultPitchAngle;
	bool bInvertPitchAxis;
	
	TSoftObjectPtr<UCurveFloat> YawCurve;
	bool bSnapYaw90;
	float DefaultYawAngle;
	bool bInvertYawAxis;
	float SnapYawDuration;
	
	TSoftObjectPtr<UCurveFloat> FOVCurve;
	float DefaultFOV = 70;
};
