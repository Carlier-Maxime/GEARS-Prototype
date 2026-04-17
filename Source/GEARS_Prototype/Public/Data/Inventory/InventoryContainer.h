#pragma once
#include "ItemStack.h"
#include "Definitions/GEARS_Delegates.h"

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
	void SetCapacity(const int32 NewCapacity);
	bool AddStack(FItemStack& Stack);
	FItemStack RemoveStack(int32 SlotIndex);
	FString ToString() const;
	
	FOnItemStackChanged OnItemGained;
	FOnItemStackChanged OnItemLost;
	FOnInt32Changed OnSlotChanged;
	FOnInt32Changed OnCapacityChanged;
	
protected:
	bool PerformAddStack(FItemStack& Stack);
	FItemStack PerformRemoveStack(int32 SlotIndex);
	
private:
	TArray<FItemStack> Stacks;
	int32 StackCount = 0;
	int32 Capacity = 0;
};
