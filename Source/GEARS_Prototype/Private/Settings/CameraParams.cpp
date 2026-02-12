#include "CameraParams.h"

#include "Settings/GridParams.h"
#include "Definitions/GEARS_Macro.h"

float CameraParams::GetMinZoomDistance() const
{
	return GetZoomDistance(GetTimeRange(ZoomFactorCurve).Get<0>());
}

float CameraParams::GetMaxZoomDistance() const
{
	return GetZoomDistance(GetTimeRange(ZoomFactorCurve).Get<1>());
}

float CameraParams::GetDefaultZoomDistance() const
{
	return GetZoomDistance(DefaultZoomDistanceFactor);
}

float CameraParams::GetZoomSpeed(const float Distance) const
{
	ensureSoftPtrOrRet(ZoomFactorCurve, -1);
	return GetZoomDistance(
		ZoomFactorCurve.LoadSynchronous()->GetFloatValue(
			Distance / GridParams::Get().GetCellSize()
		)
	);
}

bool CameraParams::IsInvertZoomAxis() const
{
	return bInvertZoomAxis;
}

float CameraParams::GetZoomDistance(const float Factor)
{
	return Factor * GridParams::Get().GetCellSize();
}

float CameraParams::GetMinPitch() const
{
	return GetTimeRange(PitchCurve).Get<0>();
}

float CameraParams::GetMaxPitch() const
{
	return GetTimeRange(PitchCurve).Get<1>();
}

float CameraParams::GetPitchSpeed(const float Pitch) const
{
	ensureSoftPtrOrRet(PitchCurve, -1);
	return PitchCurve.LoadSynchronous()->GetFloatValue(Pitch);
}

bool CameraParams::IsLockPitch() const
{
	return bLockPitch;
}

void CameraParams::SetLockPitch(const bool bActive)
{
	bLockPitch = bActive;
	OnLockPitchChanged.Broadcast(bActive);
}

float CameraParams::GetYawSpeed(float Yaw) const
{
	ensureSoftPtrOrRet(YawCurve, -1);
	return YawCurve.LoadSynchronous()->GetFloatValue(Yaw);
}

bool CameraParams::IsSnapYaw90() const
{
	return bSnapYaw90;
}

void CameraParams::SetSnapYaw90(const bool bActive)
{
	bSnapYaw90 = bActive;
	OnSnapYawStateChanged.Broadcast(bActive);
}

float CameraParams::GetMinFOV() const
{
	return GetTimeRange(FOVCurve).Get<0>();
}

float CameraParams::GetMaxFOV() const
{
	return GetTimeRange(FOVCurve).Get<1>();
}

float CameraParams::GetFOVSpeed(float FOV) const
{
	ensureSoftPtrOrRet(FOVCurve, -1);
	return FOVCurve.LoadSynchronous()->GetFloatValue(FOV);
}

float CameraParams::GetDefaultFOV() const
{
	return DefaultFOV;
}

TTuple<float, float> CameraParams::GetTimeRange(const TSoftObjectPtr<UCurveFloat>& Curve) const
{
	ensureSoftPtrOrRet(Curve, MakeTuple(-1, -1));
	float Min, Max;
	Curve.LoadSynchronous()->GetTimeRange(Min, Max);
	return {Min, Max};
}