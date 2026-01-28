// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerBase.h"

#include "UI/Styles/StyleData.h"

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
	if (!GetStyleAsset() && GetWorld() && GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ApplyStyle: No Style Asset assigned!"), *GetName());
	}
}

void UStylizerBase::LerpBrush(FSlateBrush& Brush, const FLinearColor& ColorB, const float Exponent, const bool bKeepOriginalAlpha)
{
	const auto Color = Brush.TintColor.GetSpecifiedColor();
	const auto OutlineColor = Brush.OutlineSettings.Color.GetSpecifiedColor();
	auto NewColor = FMath::Lerp(Color, ColorB, Exponent);
	auto NewOutlineColor = FMath::Lerp(OutlineColor, ColorB, Exponent);
	if (bKeepOriginalAlpha)
	{
		NewColor.A = Color.A;
		NewOutlineColor.A = OutlineColor.A;
	}
	Brush.TintColor = NewColor;
	Brush.OutlineSettings.Color = NewOutlineColor;
}

void UStylizerBase::SetBrushAlpha(FSlateBrush& Brush, const float AlphaIntensity)
{
	const auto Color = Brush.TintColor.GetSpecifiedColor();
	const auto OutlineColor = Brush.OutlineSettings.Color.GetSpecifiedColor();
	auto NewColor = Color;
	NewColor.A *= AlphaIntensity;
	auto NewOutlineColor = OutlineColor;
	NewOutlineColor.A *= AlphaIntensity;
	Brush.TintColor = NewColor;
	Brush.OutlineSettings.Color = NewOutlineColor;
}
