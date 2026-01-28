// Fill out your copyright notice in the Description page of Project Settings.


#include "StylizerTextBlock.h"

#include "Components/TextBlock.h"

void UStylizerTextBlock::ApplyStyle()
{
	Super::ApplyStyle();
	const auto TextBlock = GetWidget<UTextBlock>();
	if (!TextBlock || !Style) return;
	TextBlock->SetFont(Style->Font);
	TextBlock->SetColorAndOpacity(Style->ColorAndOpacity);
	TextBlock->SetShadowOffset(Style->ShadowOffset);
	TextBlock->SetShadowColorAndOpacity(Style->ShadowColorAndOpacity);
	if (const auto Asset = Style->StrikeBrush)
		TextBlock->SetStrikeBrush(Asset->Brush);
	TextBlock->SetTextTransformPolicy(Style->TransformPolicy);
	TextBlock->SetTextOverflowPolicy(Style->OverflowPolicy);
	TextBlock->SetJustification(Style->Justification);
	TextBlock->SetAutoWrapText(Style->AutoWrap);
	TextBlock->SetMargin(Style->Margin);
	TextBlock->SetLineHeightPercentage(Style->LineHeightPercentage);
}

UStyleData* UStylizerTextBlock::GetStyleAsset() const
{
	return Style;
}
