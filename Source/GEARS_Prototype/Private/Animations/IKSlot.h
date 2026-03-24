#pragma once
#include "GameplayTagContainer.h"
#include "IKSlot.generated.h"

USTRUCT(BlueprintType)
struct FIKSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "IK"))
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Target;
	
	bool operator==(const FIKSlot& Other) const;
};
