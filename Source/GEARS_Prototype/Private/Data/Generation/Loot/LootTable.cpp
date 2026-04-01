#include "Data/Generation/Loot/LootTable.h"

void FLootTable::GenerateLoot(FInventoryContainer& OutLoot, const FRandomStream& InStream)
{
	for (auto& Pool : Pools) Pool.Rolls(OutLoot, InStream);
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
