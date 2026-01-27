// Fill out your copyright notice in the Description page of Project Settings.


#include "StyleData.h"

void UStyleData::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	OnStyleChanged.Broadcast();
}
