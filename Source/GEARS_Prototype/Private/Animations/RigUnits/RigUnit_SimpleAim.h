#pragma once

#include "Units/RigUnit.h"
#include "ControlRig.h"
#include "Math/Axis.h"
#include "RigUnit_SimpleAim.generated.h"

USTRUCT(meta=(DisplayName="SimpleAim", Category="GEARS|Animation"))
struct FRigUnit_SimpleAim : public FRigUnitMutable
{
	GENERATED_BODY()

	RIGVM_METHOD()
	GEARS_PROTOTYPE_API virtual void Execute() override;

	UPROPERTY(meta = (Input))
	FRigElementKey Base = ERigElementType::Bone;

	UPROPERTY(meta = (Input))
	FRigElementKey Target = ERigElementType::Bone;
	
	UPROPERTY(meta = (Input))
	bool bMutualAim = false;
	
private:
	static FVector CalcBoneAxis(const URigHierarchy& Hierarchy, const FRigElementKey& Bone);
	static void Aim(URigHierarchy& Hierarchy, const FRigElementKey& Base, const FTransform& BaseT, const FTransform& TargetT);
};
