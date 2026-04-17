#include "Data/Inventory/InventoryContainer.h"

bool FInventoryContainer::AddStack(FItemStack& Stack)
{
	const auto BaseQuantity = Stack.Quantity;
	const auto Ret = PerformAddStack(Stack);
	if (const auto AddedQuantity = BaseQuantity - Stack.Quantity; AddedQuantity > 0)
		OnItemGained.Broadcast({ Stack.ItemID, AddedQuantity});
	return Ret;
}

FItemStack FInventoryContainer::RemoveStack(int32 SlotIndex)
{
	const auto Ret = PerformRemoveStack(SlotIndex);
	if (Ret.IsValid()) OnItemLost.Broadcast(Ret);
	return Ret;
}

bool FInventoryContainer::PerformAddStack(FItemStack& Stack)
{
	if (!Stack.IsValid()) return false;
	int32 FreeSlotIndex = -1;
	for (auto i=0; i<Stacks.Num(); ++i)
	{
		auto& CurrentStack = Stacks[i];
		if (FreeSlotIndex==-1 && CurrentStack.IsEmpty()) FreeSlotIndex = i;
		if (CurrentStack.ItemID == Stack.ItemID)
		{
			Stack.Quantity = CurrentStack.AddQuantity(Stack.Quantity);
			OnSlotChanged.Broadcast(i);
			if (Stack.Quantity == 0) return true;
		}
	}
	if (Stacks.IsValidIndex(FreeSlotIndex))
	{
		auto& FreeSlot = Stacks[FreeSlotIndex];
		FreeSlot.ItemID = Stack.ItemID;
		FreeSlot.Quantity = Stack.Quantity;
		Stack.Quantity = 0;
		StackCount++;
		OnSlotChanged.Broadcast(FreeSlotIndex);
		return true;
	}
	if (Stacks.Num() >= GetCapacity()) return false;
	const auto NewIndex = Stacks.Emplace(Stack);
	Stack.Quantity = 0;
	StackCount++;
	OnSlotChanged.Broadcast(NewIndex);
	return true;
}

FItemStack FInventoryContainer::PerformRemoveStack(const int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= Stacks.Num() ) return {};
	const auto RemovedStack = Stacks[SlotIndex];
	Stacks[SlotIndex].Quantity = 0;
	StackCount--;
	OnSlotChanged.Broadcast(SlotIndex);
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

void FInventoryContainer::SetCapacity(const int32 NewCapacity)
{
	Capacity=NewCapacity;
	Stacks.Reserve(NewCapacity);
	OnCapacityChanged.Broadcast(NewCapacity);
}
