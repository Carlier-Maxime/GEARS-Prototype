// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerCheckbox.h"

#include "Components/CheckBox.h"

void UStylizerCheckbox::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Checkbox = GetWidget<UCheckBox>();
	if (!Checkbox || !Style) return;
	Checkbox->SetWidgetStyle(Style->Slate);
}

UStyleData* UStylizerCheckbox::GetStyleAsset() const
{
	return Style;
}
