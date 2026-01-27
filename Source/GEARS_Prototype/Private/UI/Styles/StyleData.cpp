// Fill out your copyright notice in the Description page of Project Settings.


#include "StyleData.h"

#if WITH_EDITOR
void UStyleData::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	OnStyleChanged.Broadcast();
}
#endif