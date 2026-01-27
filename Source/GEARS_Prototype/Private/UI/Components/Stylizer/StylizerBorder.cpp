// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerBorder.h"

#include "Components/Border.h"
#include "UI/Styles/BorderStyle.h"

void UStylizerBorder::ApplyStyle()
{
	Super::ApplyStyle();
	const auto Border = GetWidget<UBorder>();
	if (!Border || !Style || !Style->BrushAsset) return;
	Border->SetBrush(Style->BrushAsset->Brush);
	Border->SetBrushColor(Style->BrushColor);
}

UStyleData* UStylizerBorder::GetStyleAsset() const
{
	return Style;
}
