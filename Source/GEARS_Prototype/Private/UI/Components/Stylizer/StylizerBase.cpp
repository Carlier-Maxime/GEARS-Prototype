// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerBase.h"

void UStylizerBase::OnPreConstruct(bool bIsDesignTime)
{
	Super::OnPreConstruct(bIsDesignTime);
	ApplyStyle();
}

#if WITH_EDITOR
void UStylizerBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ApplyStyle();
}
#endif

void UStylizerBase::ApplyStyle()
{
	ensureMsgf(GetStyleAsset(), TEXT("Style is undefined !"));
}
