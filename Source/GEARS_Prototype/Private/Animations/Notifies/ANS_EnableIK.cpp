// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_EnableIK.h"

#include "Animations/Interfaces/AnimIKInterface.h"

void UANS_EnableIK::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	auto* AnimInstance = MeshComp->GetAnimInstance();
	auto* IKInterface = Cast<IAnimIKInterface>(AnimInstance);
	if (!IKInterface) return;
	IKSlot = &IKInterface->GetIK(Tag);
	AnimInstance->GetCurveValueWithDefault(AlphaCurve, 1, IKSlot->Weight);
}

void UANS_EnableIK::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (IKSlot) MeshComp->GetAnimInstance()->GetCurveValueWithDefault(AlphaCurve, 1, IKSlot->Weight);
}

void UANS_EnableIK::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	auto* IKInterface = Cast<IAnimIKInterface>(MeshComp->GetAnimInstance());
	if (!IKInterface || !IKSlot) return;
	IKSlot->Weight = 0;
	IKSlot = nullptr;
}
