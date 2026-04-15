// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEntryWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Definitions/ItemDefinition.h"
#include "Data/Inventory/ItemStack.h"

void UItemEntryWidget::SetItem(const FItemStack& ItemStack)
{
	if (!ItemStack.IsValid()) return;
	Amount = ItemStack.Quantity;
	Item = &ItemStack.GetCheckedItem();
	RefreshAll();
}

void UItemEntryWidget::AddAmount(const int32 Value)
{
	Amount += Value;
	Amount_TextBlock->SetText(FText::AsNumber(Amount));
	RefreshAmount();
	RefreshIcon();
}

void UItemEntryWidget::RefreshAmount() const
{
	check(Amount_TextBlock);
	Amount_TextBlock->SetText(FText::AsNumber(Amount));
}

void UItemEntryWidget::RefreshIcon() const
{
	check(Icon_Image && Item)
	const auto& Visual = Item->GetVisual(Amount);
	Icon_Image->SetBrushFromSoftTexture(Visual.Icon, false);
}

void UItemEntryWidget::RefreshAll() const
{
	RefreshIcon();
	RefreshAmount();
}
