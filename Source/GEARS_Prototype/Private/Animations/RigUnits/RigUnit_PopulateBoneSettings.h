#pragma once
#include "Units/RigUnit.h"
#include "PBIK_Shared.h"
#include "RigUnit_PopulateBoneSettings.generated.h"

USTRUCT(meta=(DisplayName="Populate BoneSettings from Controls", Category="GEARS|Animation"))
struct FRigUnit_PopulateBoneSettings : public FRigUnitMutable
{
	GENERATED_BODY()
	
	RIGVM_METHOD()
	GEARS_PROTOTYPE_API virtual void Execute() override;
	
	UPROPERTY(meta = (Input, Output))
	TArray<FPBIKBoneSetting> Settings;
	
private:
	static EPBIKLimitType GetLimitType(const FRigControlLimitEnabled& Limit, float Min, float Max);
};
