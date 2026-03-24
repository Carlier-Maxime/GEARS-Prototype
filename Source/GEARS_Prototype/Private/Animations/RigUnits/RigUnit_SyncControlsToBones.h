#pragma once
#include "Units/RigUnit.h"
#include "RigUnit_SyncControlsToBones.generated.h"

USTRUCT(meta=(DisplayName="SyncControlsToBones", Category="GEARS|Animation"))
struct FRigUnit_SyncControlsToBones : public FRigUnitMutable
{
	GENERATED_BODY()
	
	RIGVM_METHOD()
	virtual void Execute() override;
	
	static FTransform AverageTransforms(const TArray<FTransform>& Transforms);
};
