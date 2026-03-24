// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animations/IKSlot.h"
#include "ANS_EnableIK.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UANS_EnableIK : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK", meta=(Categories = "IK"))
	FGameplayTag Tag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FName AlphaCurve;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	
private:
	FIKSlot* IKSlot;
};
