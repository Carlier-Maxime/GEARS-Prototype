// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "HUDWidget.h"
#include "ItemEntryPayload.h"
#include "ItemEntryWidget.h"
#include "Components/ListView.h"
#include "Data/Inventory/InventoryContainer.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Inventory = UHUDWidget::GetInventory(GetOwningPlayerPawn());
	if (!Inventory) return;
	Inventory->OnSlotChanged.AddUObject(this, &ThisClass::OnSlotChanged);
	Inventory->OnCapacityChanged.AddUObject(this, &ThisClass::OnCapacityChanged);
	CompleteMissingSlots();
}

void UInventoryWidget::NativeDestruct()
{
	if (Inventory)
	{
		Inventory->OnSlotChanged.RemoveAll(this);
		Inventory->OnCapacityChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UInventoryWidget::OnSlotChanged(int SlotIndex)
{
	if (SlotIndex >= Grid->GetNumItems())
	{
		CompleteMissingSlots();
		return;
	}
	const auto& Stack = Inventory->GetStack(SlotIndex);
	auto* Payload = Cast<UItemEntryPayload>(Grid->GetItemAt(SlotIndex));
	if (!Payload) return;
	Payload->Stack = &Stack;
	Grid->GetEntryWidgetFromItem<UItemEntryWidget>(Payload)->SetItem(Stack);
}

void UInventoryWidget::CompleteMissingSlots()
{
	for (auto i=Grid->GetNumItems(); i<Inventory->GetStacks().Num(); ++i)
	{
		auto* Payload = NewObject<UItemEntryPayload>();
		Payload->Stack = &Inventory->GetStack(i);
		Grid->AddItem(Payload);
	}
	for (auto i=Grid->GetNumItems(); i<Inventory->GetCapacity(); ++i)
	{
		auto* Payload = NewObject<UItemEntryPayload>();
		Payload->Stack = nullptr;
		Grid->AddItem(Payload);
	}
}

void UInventoryWidget::OnCapacityChanged(int I)
{
	CompleteMissingSlots();
}
