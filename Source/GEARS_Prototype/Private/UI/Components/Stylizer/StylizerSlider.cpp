// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerSlider.h"

#include "Components/Slider.h"

void UStylizerSlider::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Slider = GetWidget<USlider>();
	if (!Slider || !Style) return;
	Slider->SetWidgetStyle(Style->Slate);
}

UStyleData* UStylizerSlider::GetStyleAsset() const
{
	return Style;
}
