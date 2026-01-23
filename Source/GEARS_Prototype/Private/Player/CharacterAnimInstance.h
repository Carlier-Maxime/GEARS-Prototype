// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void PreUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float AnimReferenceSpeed = 300.f;

private:
	UPROPERTY()
	TObjectPtr<APawn> Pawn;
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float MovingPlayRate;
};
