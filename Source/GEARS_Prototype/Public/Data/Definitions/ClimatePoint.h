#pragma once

#include "ClimatePoint.generated.h"

USTRUCT()
struct FClimatePoint
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Temperature", meta = (ClampMin = 0, ClampMax = 1))
	float Temperature = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Temperature", meta= (ClampMin = 0, UIMax = 2))
	float TempWeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Humidity", meta = (ClampMin = 0, ClampMax = 1))
	float Humidity = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Humidity", meta= (ClampMin = 0, UIMax = 2))
	float HumWeight = 1.0f;
	
	FORCEINLINE float ManhattanDistance(const float Temp, const float Hum) const
	{
		return FMath::Abs(Temperature - Temp) * TempWeight + FMath::Abs(Humidity - Hum) * HumWeight;
	}
};