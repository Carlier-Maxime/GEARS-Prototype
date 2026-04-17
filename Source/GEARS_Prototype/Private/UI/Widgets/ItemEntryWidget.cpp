// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEntryWidget.h"

#include "ItemEntryPayload.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Definitions/ItemDefinition.h"
#include "Data/Inventory/ItemStack.h"

void UItemEntryWidget::SetItem(const FItemStack& ItemStack)
{
	if (!ItemStack.IsValid()) return;
	Amount = ItemStack.Quantity;
	Item = &ItemStack.GetCheckedItem();
	ItemID = ItemStack.ItemID;
	RefreshAll();
}

void UItemEntryWidget::AddAmount(const int32 Value)
{
	Amount += Value;
	Amount_TextBlock->SetText(FText::AsNumber(Amount));
	RefreshAmount();
	RefreshIcon();
}

void UItemEntryWidget::RefreshAmount_Internal() const
{
	check(Amount_TextBlock);
	Amount_TextBlock->SetText(FText::AsNumber(Amount));
	OnRefresh.Broadcast();
}

void UItemEntryWidget::RefreshIcon_Internal() const
{
	check(Icon_Image && Item)
	const auto& Visual = Item->GetVisual(Amount);
	Icon_Image->SetBrushFromSoftTexture(Visual.Icon, false);
	OnRefresh.Broadcast();
}

void UItemEntryWidget::RefreshAll()
{
	RefreshIcon();
	RefreshAmount();
}

void UItemEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const auto* Payload = Cast<UItemEntryPayload>(ListItemObject);
	if (!Payload || !Payload->Stack) return;
	SetItem(*Payload->Stack);
}
