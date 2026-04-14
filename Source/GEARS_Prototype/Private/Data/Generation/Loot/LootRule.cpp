#include "Data/Generation/Loot/LootRule.h"

void FLootRule::RollsFromDamage(float& AccDamage, FInventoryContainer& OutLoot, const FRandomStream& InStream) const
{
	if (!bRollPerDamage) return;
	while (AccDamage >= DamageRequiredPerRoll)
	{
		Table.GenerateLoot(OutLoot, InStream);
		AccDamage -= DamageRequiredPerRoll;
	}
}

#if WITH_EDITOR
void FLootRule::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent,
	FEditPropertyChain::TDoubleLinkedListNode* CurrentNode)
{
	if (!CurrentNode) return;
	if (CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(FLootRule, Table))
	{
		Table.PostEditChangeChainProperty(PropertyChangedEvent, CurrentNode->GetNextNode());
	}
}
#endif
