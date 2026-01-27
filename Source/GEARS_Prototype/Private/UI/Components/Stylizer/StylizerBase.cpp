// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerBase.h"

#include "UI/Styles/StyleData.h"

UStylizerBase::UStylizerBase()
{
	SetFlags(RF_Transient);
	ClearFlags(RF_Transactional);
}

void UStylizerBase::BeginDestroy()
{
	if (const auto Asset = GetStyleAsset())
	{
		Asset->OnStyleChanged.RemoveAll(this);
	}
	Super::BeginDestroy();
}

void UStylizerBase::OnPreConstruct(bool bIsDesignTime)
{
	Super::OnPreConstruct(bIsDesignTime);
	if (const auto Asset = GetStyleAsset())
	{
		Asset->OnStyleChanged.AddWeakLambda(this, [this]()
		{
			if (IsValid(this)) ApplyStyle();
		});
	}
	ApplyStyle();
}

#if WITH_EDITOR
void UStylizerBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (const auto Asset = GetStyleAsset())
	{
		Asset->OnStyleChanged.AddWeakLambda(this, [this]()
		{
			if (IsValid(this)) ApplyStyle();
		});
	}
	ApplyStyle();
}
#endif

void UStylizerBase::ApplyStyle()
{
	ensureMsgf(GetStyleAsset(), TEXT("Style is undefined !"));
}
