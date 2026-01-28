// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerSlider.h"

#include "Components/Slider.h"

void UStylizerSlider::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Slider = GetWidget<USlider>();
	if (!Slider || !Style) return;
	auto NewStyle = Slider->GetWidgetStyle();
	if (const auto Asset = Style->BarBrush)
	{
		auto BaseBrush = Asset->Brush;
		NewStyle.SetNormalBarImage(BaseBrush);
		LerpBrush(BaseBrush, FLinearColor::White, Style->HoverLightIntensity);
		NewStyle.SetHoveredBarImage(BaseBrush);
		SetBrushAlpha(BaseBrush, Style->PressedDarkIntensity);
		NewStyle.SetDisabledBarImage(BaseBrush);
	}
	if (const auto Asset = Style->ThumbBrush)
	{
		auto BaseBrush = Asset->Brush;
		NewStyle.SetNormalThumbImage(BaseBrush);
		LerpBrush(BaseBrush, FLinearColor::White, Style->HoverLightIntensity);
		NewStyle.SetHoveredThumbImage(BaseBrush);
		SetBrushAlpha(BaseBrush, Style->PressedDarkIntensity);
		NewStyle.SetDisabledThumbImage(BaseBrush);
	}
	NewStyle.SetBarThickness(Style->BarThickness);
	Slider->SetWidgetStyle(NewStyle);
}

UStyleData* UStylizerSlider::GetStyleAsset() const
{
	return Style;
}
