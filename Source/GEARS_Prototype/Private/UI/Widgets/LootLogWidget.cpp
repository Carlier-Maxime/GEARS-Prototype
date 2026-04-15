// Fill out your copyright notice in the Description page of Project Settings.


#include "LootLogWidget.h"

#include "ItemEntryWidget.h"
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
	EntryCached.Reserve(FMath::Min(4, MaxLog));
}

void ULootLogWidget::NativeDestruct()
{
	if (Inventory) Inventory->OnItemGained.RemoveAll(this);
	for (auto& [Key, LogEntry] : LogsMap)
	{
		GetWorld()->GetTimerManager().ClearTimer(LogEntry.Timer);
		LogsPanel->RemoveChild(LogEntry.Widget);
	}
	LogsMap.Empty();
	EntryCached.Empty();
	Super::NativeDestruct();
}

void ULootLogWidget::OnItemGained(const FItemStack& ItemStack)
{
	const int32 Key = ItemStack.ItemID;
	if (auto* EntryPtr = LogsMap.Find(ItemStack.ItemID); EntryPtr)
	{
		EntryPtr->Widget->AddAmount(ItemStack.Quantity);
		GetWorld()->GetTimerManager().ClearTimer(EntryPtr->Timer);
		GetWorld()->GetTimerManager().SetTimer(EntryPtr->Timer, [this, Key]
		{
			ClearLog(Key);
		}, LogDuration, false);
		return;
	}
	if (LogsMap.Num() >= MaxLog) return;
	auto Entry = CreateEntryWidget(ItemStack);
	if (!Entry)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create entry for item %s"), *ItemStack.ToString());
		return;
	}
	auto& LogEntry = LogsMap.Add(ItemStack.ItemID, {Entry});
	LogsPanel->AddChild(Entry);
	GetWorld()->GetTimerManager().SetTimer(LogEntry.Timer, [this, Key]
	{
		ClearLog(Key);
	}, LogDuration, false);
}

TObjectPtr<UItemEntryWidget> ULootLogWidget::CreateEntryWidget(const FItemStack& ItemStack)
{
	TObjectPtr<UItemEntryWidget> Entry;
	if (EntryCached.IsEmpty()) Entry = CreateWidget<UItemEntryWidget>(this, EntryClass);
	else Entry = EntryCached.Pop(EAllowShrinking::No);
	if (!ItemStack.IsValid()) return Entry;
	Entry->SetItem(ItemStack);
	return Entry;
}

void ULootLogWidget::ClearLog(const int32 Key)
{
	auto LogEntry = LogsMap.FindAndRemoveChecked(Key);
	EntryCached.Add(LogEntry.Widget);
	LogsPanel->RemoveChild(LogEntry.Widget);
	GetWorld()->GetTimerManager().ClearTimer(LogEntry.Timer);
	LogEntry.Timer.Invalidate();
}
