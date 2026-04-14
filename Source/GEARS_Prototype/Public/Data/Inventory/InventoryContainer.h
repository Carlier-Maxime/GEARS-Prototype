#pragma once
#include "ItemStack.h"

class GEARS_PROTOTYPE_API FInventoryContainer
{
	
public:
	virtual ~FInventoryContainer() = default;
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
	
private:
	TArray<FItemStack> Stacks;
	int32 StackCount = 0;
	int32 Capacity = 0;
};
