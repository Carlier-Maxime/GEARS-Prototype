// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerBase.h"

#include "UI/Styles/StyleData.h"

void UStylizerBase::OnPreConstruct(bool bIsDesignTime)
{
	Super::OnPreConstruct(bIsDesignTime);
	if (const auto Asset = GetStyleAsset())
	{
		Asset->OnStyleChanged.AddUniqueDynamic(this, &ThisClass::ApplyStyle);
	}
	ApplyStyle();
}

#if WITH_EDITOR
void UStylizerBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (const auto Asset = GetStyleAsset())
	{
		Asset->OnStyleChanged.AddUniqueDynamic(this, &ThisClass::ApplyStyle);
	}
	ApplyStyle();
}
#endif

void UStylizerBase::ApplyStyle()
{
	ensureMsgf(GetStyleAsset(), TEXT("Style is undefined !"));
}
