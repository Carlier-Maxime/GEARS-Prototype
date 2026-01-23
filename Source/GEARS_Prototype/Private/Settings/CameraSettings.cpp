// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSettings.h"

#include "GridSettings.h"

float UCameraSettings::GetMinZoomDistance() const
{
	return GetZoomDistance(MinZoomDistanceFactor);
}

float UCameraSettings::GetMaxZoomDistance() const
{
	return GetZoomDistance(MaxZoomDistanceFactor);
}

float UCameraSettings::GetDefaultZoomDistance() const
{
	return GetZoomDistance(DefaultZoomDistanceFactor);
}

float UCameraSettings::GetZoomDistance(const float Factor)
{
	return Factor * GetDefault<UGridSettings>()->GetCellSize();
}
