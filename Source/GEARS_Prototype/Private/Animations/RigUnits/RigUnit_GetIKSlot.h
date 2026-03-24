#pragma once
#include "Animations/IKSlot.h"
#include "Units/RigUnit.h"
#include "RigUnit_GetIKSlot.generated.h"

USTRUCT(meta=(DisplayName="Get IKSlot", Category="GEARS|Animation"))
struct FRigUnit_GetIKSlot : public FRigUnit
{
	GENERATED_BODY()
	
	RIGVM_METHOD()
	GEARS_PROTOTYPE_API virtual void Execute() override;
	
	UPROPERTY(Transient, meta = (Input))
	FGameplayTag Tag;
	
	UPROPERTY(Transient, meta = (Output))
	FIKSlot Slot;
};
