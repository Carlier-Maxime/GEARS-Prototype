// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEntryWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Definitions/ItemDefinition.h"

void UItemEntryWidget::SetItem(const FItemDefinition& Item, int32 Amount)
{
	check(Amount_TextBlock && Icon_Image)
	Amount_TextBlock->SetText(FText::AsNumber(Amount));
	const auto& Visual = Item.GetVisual(Amount);
	Icon_Image->SetBrushFromSoftTexture(Visual.Icon, false);
}
