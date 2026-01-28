// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerButton.h"

#include "Components/Button.h"

void UStylizerButton::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Button = GetWidget<UButton>();
	if (!Button || !Style) return;
	Button->SetStyle(Style->Slate);
}

UStyleData* UStylizerButton::GetStyleAsset() const
{
	return Style;
}
