// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerButton.h"

#include "Components/Button.h"

void UStylizerButton::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Button = GetWidget<UButton>();
	if (!Button || !Style) return;
	Button->SetColorAndOpacity(Style->ForegroundColor);
	Button->SetBackgroundColor(FLinearColor::White);
	auto NewStyle = Button->GetStyle();
	FSlateSound NewSound;
	NewSound.SetResourceObject(Style->PressedSound);
	NewStyle.SetPressedSound(NewSound);
	NewSound.SetResourceObject(Style->ClickedSound);
	NewStyle.SetClickedSound(NewSound);
	NewSound.SetResourceObject(Style->HoveredSound);
	NewStyle.SetHoveredSound(NewSound);
	NewStyle.SetNormalPadding(Style->Padding);
	NewStyle.SetPressedPadding(Style->Padding);
	if (const auto Asset = Style->MainBrush)
	{
		auto BaseBrush = Asset->Brush;
		NewStyle.SetNormal(BaseBrush);
		LerpBrush(BaseBrush, FLinearColor::White, Style->HoverLightIntensity);
		NewStyle.SetHovered(BaseBrush);
		LerpBrush(BaseBrush, FLinearColor::Black, Style->PressedDarkIntensity);
		NewStyle.SetPressed(BaseBrush);
		SetBrushAlpha(BaseBrush, Style->DisabledAlphaIntensity);
		NewStyle.SetDisabled(BaseBrush);
	}
	Button->SetStyle(NewStyle);
}

UStyleData* UStylizerButton::GetStyleAsset() const
{
	return Style;
}
