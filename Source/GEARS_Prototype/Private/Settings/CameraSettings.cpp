// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSettings.h"

#include "GridParams.h"
#include "Definitions/GEARS_Macro.h"
#include "Curves/CurveFloat.h"

float UCameraSettings::GetMinZoomDistance() const
{
	return GetZoomDistance(GetTimeRange(ZoomFactorCurve).Get<0>());
}

float UCameraSettings::GetMaxZoomDistance() const
{
	return GetZoomDistance(GetTimeRange(ZoomFactorCurve).Get<1>());
}

float UCameraSettings::GetDefaultZoomDistance() const
{
	return GetZoomDistance(DefaultZoomDistanceFactor);
}

float UCameraSettings::GetZoomSpeed(const float Distance) const
{
	ensureSoftPtrOrRet(ZoomFactorCurve, -1);
	return GetZoomDistance(
		ZoomFactorCurve.LoadSynchronous()->GetFloatValue(
			Distance / GridParams::GetCellSize()
		)
	);
}

float UCameraSettings::GetZoomDistance(const float Factor)
{
	return Factor * GridParams::GetCellSize();
}

float UCameraSettings::GetMinPitch() const
{
	return GetTimeRange(PitchCurve).Get<0>();
}

float UCameraSettings::GetMaxPitch() const
{
	return GetTimeRange(PitchCurve).Get<1>();
}

float UCameraSettings::GetPitchSpeed(const float Pitch) const
{
	ensureSoftPtrOrRet(PitchCurve, -1);
	return PitchCurve.LoadSynchronous()->GetFloatValue(Pitch);
}

bool UCameraSettings::IsLockPitch() const
{
	return bLockPitch;
}

void UCameraSettings::SetLockPitch(const bool bActive)
{
	bLockPitch = bActive;
	OnLockPitchChanged.Broadcast(bActive);
}

float UCameraSettings::GetYawSpeed(float Yaw) const
{
	ensureSoftPtrOrRet(YawCurve, -1);
	return YawCurve.LoadSynchronous()->GetFloatValue(Yaw);
}

bool UCameraSettings::IsSnapYaw90() const
{
	return bSnapYaw90;
}

void UCameraSettings::SetSnapYaw90(const bool bActive)
{
	bSnapYaw90 = bActive;
	OnSnapYawStateChanged.Broadcast(bActive);
}

float UCameraSettings::GetMinFOV() const
{
	return GetTimeRange(FOVCurve).Get<0>();
}

float UCameraSettings::GetMaxFOV() const
{
	return GetTimeRange(FOVCurve).Get<1>();
}

float UCameraSettings::GetFOVSpeed(float FOV) const
{
	ensureSoftPtrOrRet(FOVCurve, -1);
	return FOVCurve.LoadSynchronous()->GetFloatValue(FOV);
}

float UCameraSettings::GetDefaultFOV() const
{
	return DefaultFOV;
}

TTuple<float, float> UCameraSettings::GetTimeRange(const TSoftObjectPtr<UCurveFloat>& Curve) const
{
	ensureSoftPtrOrRet(Curve, MakeTuple(-1, -1));
	float Min, Max;
	Curve.LoadSynchronous()->GetTimeRange(Min, Max);
	return {Min, Max};
}
