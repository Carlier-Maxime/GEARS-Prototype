#pragma once

#include "CoreMinimal.h"
#include "ProcSpawnData.generated.h"

USTRUCT()
struct FProcSpawnData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int16 ResourceTypeIndex = -1;
	UPROPERTY()
	FTransform Transform = FTransform::Identity;
};
