#pragma once
#include "ItemStack.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, int32 Slot)

class GEARS_PROTOTYPE_API FInventoryContainer
{
	
public:
	virtual ~FInventoryContainer() = default;
	FORCEINLINE const FItemStack& GetStack(const int32 SlotIndex) const { return Stacks[SlotIndex]; }
	FORCEINLINE const TArray<FItemStack>& GetStacks() const { return Stacks; }
	FORCEINLINE void Clear() { Stacks.Empty(); }
	FORCEINLINE bool IsEmpty() const { return StackCount == 0; }
	FORCEINLINE bool IsFull() const { return StackCount >= GetCapacity(); }
	FORCEINLINE int32 GetStackCount() const { return StackCount; }
	FORCEINLINE int32 GetCapacity() const { return Capacity; }
	FORCEINLINE void SetCapacity(const int32 NewCapacity) { Capacity=NewCapacity; Stacks.Reserve(NewCapacity); }
	bool AddStack(FItemStack& Stack);
	FItemStack RemoveStack(int32 SlotIndex);
	FString ToString() const;
	
	FOnItemStackChanged OnItemGained;
	FOnItemStackChanged OnItemLost;
	FOnInventorySlotChanged OnSlotChanged;
	
protected:
	bool PerformAddStack(FItemStack& Stack);
	FItemStack PerformRemoveStack(int32 SlotIndex);
	
private:
	TArray<FItemStack> Stacks;
	int32 StackCount = 0;
	int32 Capacity = 0;
};
