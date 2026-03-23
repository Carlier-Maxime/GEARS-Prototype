#pragma once
#include "Units/RigUnit.h"
#include "RigUnit_SimpleApplyControls.generated.h"

USTRUCT(meta=(DisplayName="SimpleApplyControls", Category="GEARS|Animation"))
struct FRigUnit_SimpleApplyControls : public FRigUnitMutable
{
	GENERATED_BODY()

	RIGVM_METHOD()
	GEARS_PROTOTYPE_API virtual void Execute() override;
};
