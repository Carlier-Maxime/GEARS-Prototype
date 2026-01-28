// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerCheckbox.h"

#include "Components/CheckBox.h"

void UStylizerCheckbox::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Checkbox = GetWidget<UCheckBox>();
	if (!Checkbox || !Style) return;
	auto NewStyle = Checkbox->GetWidgetStyle();
	if (const auto Asset = Style->MainBrush)
	{
		const auto& BaseBrush = Asset->Brush;
		auto UncheckedBrush = BaseBrush;
		NewStyle.SetUncheckedImage(UncheckedBrush);
		LerpBrush(UncheckedBrush, FLinearColor::White, Style->HoverLightIntensity);
		NewStyle.SetUncheckedHoveredImage(UncheckedBrush);
		LerpBrush(UncheckedBrush, FLinearColor::Black, Style->PressedDarkIntensity);
		NewStyle.SetUncheckedPressedImage(UncheckedBrush);
		
		auto CheckedBrush = BaseBrush;
		CheckedBrush.SetResourceObject(Style->CheckedImage);
		if (Style->bCheckedTintWhite) CheckedBrush.TintColor = FLinearColor::White;
		NewStyle.SetCheckedImage(CheckedBrush);
		LerpBrush(CheckedBrush, FLinearColor::White, Style->HoverLightIntensity);
		NewStyle.SetCheckedHoveredImage(CheckedBrush);
		LerpBrush(CheckedBrush, FLinearColor::Black, Style->PressedDarkIntensity);
		NewStyle.SetCheckedPressedImage(CheckedBrush);
		
		auto UndeterminedBrush = BaseBrush;
		UndeterminedBrush.SetResourceObject(Style->UndeterminedImage);
		SetBrushAlpha(UncheckedBrush, Style->UndeterminedAlphaIntensity);
		NewStyle.SetUndeterminedImage(UndeterminedBrush);
		LerpBrush(UndeterminedBrush, FLinearColor::White, Style->HoverLightIntensity);
		NewStyle.SetUndeterminedHoveredImage(UndeterminedBrush);
		LerpBrush(UndeterminedBrush, FLinearColor::Black, Style->PressedDarkIntensity);
		NewStyle.SetUndeterminedPressedImage(UndeterminedBrush);
	}
	NewStyle.SetPadding(Style->Padding);
	FSlateSound NewSound;
	NewSound.SetResourceObject(Style->CheckedSound);
	NewStyle.SetCheckedSound(NewSound);
	NewSound.SetResourceObject(Style->UncheckedSound);
	NewStyle.SetUncheckedSound(NewSound);
	NewSound.SetResourceObject(Style->HoveredSound);
	NewStyle.SetHoveredSound(NewSound);
	Checkbox->SetWidgetStyle(NewStyle);
}

UStyleData* UStylizerCheckbox::GetStyleAsset() const
{
	return Style;
}
