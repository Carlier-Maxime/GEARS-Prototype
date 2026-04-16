// Fill out your copyright notice in the Description page of Project Settings.


#include "LootLogWidget.h"

#include "ItemLogEntryWidget.h"
#include "Components/PanelWidget.h"
#include "Data/Inventory/ItemStack.h"

#include "Player/Components/InventoryComponent.h"

void ULootLogWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	const auto Pawn = GetOwningPlayerPawn();
	if (!ensure(Pawn)) return;
	auto* InvComp = Pawn->FindComponentByClass<UInventoryComponent>();
	Inventory = InvComp ? &InvComp->GetInventory() : nullptr;
	if (!Inventory) return;
	Inventory->OnItemGained.AddUObject(this, &ThisClass::OnItemGained);
	EntryCached.Reserve(MaxLog);
	LogsMap.Reserve(MaxLog);
}

void ULootLogWidget::NativeDestruct()
{
	if (Inventory) Inventory->OnItemGained.RemoveAll(this);
	LogsMap.Empty();
	EntryCached.Empty();
	Super::NativeDestruct();
}

void ULootLogWidget::OnItemGained(const FItemStack& ItemStack)
{
	if (const auto* EntryPtr = LogsMap.Find(ItemStack.ItemID); EntryPtr)
	{
		const auto Entry = *EntryPtr;
		Entry->AddAmount(ItemStack.Quantity);
		return;
	}
	if (LogsMap.Num() >= MaxLog) return;
	auto Entry = CreateEntryWidget(ItemStack);
	if (!Entry)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create entry for item %s"), *ItemStack.ToString());
		return;
	}
	LogsMap.Add(ItemStack.ItemID, Entry);
}

TObjectPtr<UItemLogEntryWidget> ULootLogWidget::CreateEntryWidget(const FItemStack& ItemStack)
{
	TObjectPtr<UItemLogEntryWidget> Entry;
	if (!ItemStack.IsValid()) return nullptr;
	if (EntryCached.IsEmpty())
	{
		Entry = CreateWidget<UItemLogEntryWidget>(this, EntryClass);
		if (!Entry) return nullptr;
		Entry->OnLogExpired.AddUObject(this, &ThisClass::OnLogExpired);
		Entry->SetTimeLeft(LogDuration);
		LogsPanel->AddChild(Entry);
	} else Entry = EntryCached.Pop(EAllowShrinking::No);
	Entry->SetItem(ItemStack);
	return Entry;
}

void ULootLogWidget::OnLogExpired(TObjectPtr<UItemLogEntryWidget> ItemLogEntryWidget)
{
	if (!ItemLogEntryWidget) return;
	const auto ID = ItemLogEntryWidget->GetItemID();
	if (!LogsMap.Contains(ID)) return;
	EntryCached.Add(LogsMap.FindAndRemoveChecked(ID));
}
