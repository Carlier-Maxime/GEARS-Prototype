// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Pawn = TryGetPawnOwner();
}

void UCharacterAnimInstance::PreUpdateAnimation(float DeltaSeconds)
{
	Super::PreUpdateAnimation(DeltaSeconds);
	if (!Pawn) return;
	Speed = Pawn->GetVelocity().Size2D();
}

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	bIsMoving = Speed > 3.f;
	if (bIsMoving) MovingPlayRate = Speed / AnimReferenceSpeed;
}
