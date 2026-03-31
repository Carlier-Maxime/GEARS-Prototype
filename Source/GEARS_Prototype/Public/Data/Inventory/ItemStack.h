#pragma once
#include "Data/Registry/ItemRegistry.h"
#include "Settings/GridParams.h"

struct FItemStack
{
	int16 ItemID = FItemRegistry::INVALID_INDEX;
	int32 Quantity = 0;
	
	FORCEINLINE bool IsValidID() const { return ItemID != FItemRegistry::INVALID_INDEX; }
	FORCEINLINE bool IsEmpty() const { return Quantity <= 0; }
	FORCEINLINE bool IsFull() const { return IsValidID() && Quantity >= GetCheckedItem().StackSize; }
	FORCEINLINE bool IsValid() const { return IsValidID() && !IsEmpty() && Quantity <= GetCheckedItem().StackSize; }
	FORCEINLINE const FItemDefinition& GetCheckedItem() const
	{
		check(IsValidID());
		return GridParams::Get().GetItemRegistry()[ItemID];
	}
	FORCEINLINE int32 AddQuantity(const int32 Amount)
	{
		check(Amount>=0);
		if (!IsValidID()) return Amount;
		const auto ToAdd = FMath::Min(Amount, FMath::Max(0, GetCheckedItem().StackSize - Quantity));
		Quantity += ToAdd;
		return Amount - ToAdd;
	}
	FORCEINLINE int32 RemoveQuantity(const int32 Amount)
	{
		check(Amount>=0);
		const auto ToRemove = FMath::Min(Amount, FMath::Max(0, Quantity));
		Quantity -= ToRemove;
		return Amount - ToRemove;
	}
};
