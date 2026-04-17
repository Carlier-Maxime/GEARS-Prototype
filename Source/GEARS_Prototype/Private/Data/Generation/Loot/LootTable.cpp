#include "Data/Generation/Loot/LootTable.h"

#include "Data/Inventory/InventoryContainer.h"

void FLootTable::GenerateLoot(FInventoryContainer& OutLoot, const FRandomStream& InStream) const
{
	for (auto& Pool : Pools) Pool.Rolls(OutLoot, InStream);
}

void FLootTable::GenerateLoot(FInventoryContainer& OutLoot, const FRandomStream& InStream, float Ratio) const
{
	if (FMath::IsNearlyEqual(Ratio, 1.0f)) return GenerateLoot(OutLoot, InStream);
	if (Ratio <= 0) return;
	FInventoryContainer Loots;
	Loots.SetCapacity(TNumericLimits<int32>::Max(), false);
	GenerateLoot(Loots, InStream);
	for (const auto& Loot : Loots.GetStacks())
	{
		if (!Loot.IsValidID()) continue;
		float ScaledQuantity = Loot.Quantity * Ratio;
		FItemStack Stack{Loot.ItemID, FMath::FloorToInt32(ScaledQuantity) + (InStream.FRand() < FMath::Fractional(ScaledQuantity))};
		if (Stack.IsEmpty()) continue;
		if (!OutLoot.AddStack(Stack)) UE_LOG(LogTemp, Warning, TEXT("Failed to add loot item '%s', Remaining Quantity: %d"), *Stack.GetCheckedItem().Tag.ToString(), Stack.Quantity);
	}
}

#if WITH_EDITOR
void FLootTable::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode)
{
	if (!CurrentNode) return;
	if (CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(FLootTable, Pools))
	{
		auto Index = PropertyChangedEvent.GetArrayIndex(CurrentNode->GetValue()->GetName());
		if (Index == INDEX_NONE) return;
		Pools[Index].PostEditChangeChainProperty(PropertyChangedEvent, CurrentNode->GetNextNode());
	}
}
#endif
