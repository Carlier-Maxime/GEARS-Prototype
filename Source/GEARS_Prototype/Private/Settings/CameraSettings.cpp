// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSettings.h"

#include "CameraParams.h"

void UCameraSettings::PostInitProperties()
{
	Super::PostInitProperties();
	auto& Params = CameraParams::Get();
	Params.DefaultPitchAngle = DefaultPitchAngle;
	Params.bInvertPitchAxis = bInvertPitchAxis;
	Params.PitchCurve = PitchCurve;
	Params.SetLockPitch(bLockPitch);
	
	Params.DefaultYawAngle = DefaultYawAngle;
	Params.bInvertYawAxis = bInvertYawAxis;
	Params.SnapYawDuration = SnapYawDuration;
	Params.YawCurve = YawCurve;
	Params.SetSnapYaw90(bSnapYaw90);
	
	Params.ZoomFactorCurve = ZoomFactorCurve;
	Params.DefaultZoomDistanceFactor = DefaultZoomDistanceFactor;
	Params.bInvertZoomAxis = bInvertZoomAxis;
	
	Params.DefaultFOV = DefaultFOV;
	Params.FOVCurve = FOVCurve;
}
