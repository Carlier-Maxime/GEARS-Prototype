// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return EventTag == FGameplayTag::EmptyTag ? "SendGameplayEvent" : EventTag.ToString();
}

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp || EventTag == FGameplayTag::EmptyTag) return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	if (!ASC) return;
	FGameplayEventData Payload;
	Payload.Instigator = Owner;
	ASC->HandleGameplayEvent(EventTag, &Payload);
}
