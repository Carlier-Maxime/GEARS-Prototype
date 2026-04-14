#include "Data/Inventory/InventoryContainer.h"

bool FInventoryContainer::AddStack(FItemStack& Stack)
{
	if (!Stack.IsValid()) return false;
	FItemStack* FreeSlot = nullptr;
	for (auto& CurrentStack : Stacks)
	{
		if (!FreeSlot && CurrentStack.IsEmpty()) FreeSlot = &CurrentStack;
		if (CurrentStack.ItemID == Stack.ItemID)
		{
			Stack.Quantity = CurrentStack.AddQuantity(Stack.Quantity);
			if (Stack.Quantity == 0) return true;
		}
	}
	if (FreeSlot)
	{
		FreeSlot->ItemID = Stack.ItemID;
		FreeSlot->Quantity = Stack.Quantity;
		Stack.Quantity = 0;
		StackCount++;
		return true;
	}
	if (Stacks.Num() >= GetCapacity()) return false;
	Stacks.Emplace(Stack);
	Stack.Quantity = 0;
	StackCount++;
	return true;
}

FItemStack FInventoryContainer::RemoveStack(const int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= Stacks.Num() ) return {};
	const auto RemovedStack = Stacks[SlotIndex];
	Stacks[SlotIndex].Quantity = 0;
	StackCount--;
	return RemovedStack;
}

FString FInventoryContainer::ToString() const
{
	TStringBuilder<1024> Builder;
	Builder.Appendf(TEXT("InventoryContainer: %d/%d : {"), StackCount, GetCapacity());
	if (!Stacks.IsEmpty()) Builder.Append(Stacks[0].ToString());
	for (auto& Stack : TArrayView(Stacks.GetData() + 1, Stacks.Num() - 1))
	{
		Builder.Append(TEXT(", "));
		Builder.Append(Stack.ToString());
	}
	Builder.Append(TEXT("}"));
	return Builder.ToString();
}
