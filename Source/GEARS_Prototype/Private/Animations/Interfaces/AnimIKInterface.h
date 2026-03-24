#pragma once
#include "Animations/IKSlot.h"

#include "AnimIKInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAnimIKInterface : public UInterface
{
	GENERATED_BODY()
};

class GEARS_PROTOTYPE_API IAnimIKInterface
{
	GENERATED_BODY()
public:
	FIKSlot& GetIK(FGameplayTag Tag);
	
private:
	TMap<FGameplayTag, FIKSlot> IKSlots;
};