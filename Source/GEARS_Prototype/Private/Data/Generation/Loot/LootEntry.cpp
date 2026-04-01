#include "Data/Generation/Loot/LootEntry.h"
#include "Data/Inventory/InventoryContainer.h"

void FLootEntry::Roll(FInventoryContainer& OutLoot, const FRandomStream& InStream) const
{
	if (InStream.FRand() >= DropChance) return;
	FItemStack Stack(
		GridParams::Get().GetItemRegistry().GetIndex(Item),
		InStream.RandRange(MinQuantity, MaxQuantity)
	);
	if (OutLoot.AddStack(Stack)) return;
	UE_LOG(LogTemp, Warning, TEXT("Failed to add loot item '%s', Remaining Quantity: %d"), *Item.ToString(), Stack.Quantity);
}

#if WITH_EDITOR
void FLootEntry::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent,
	FEditPropertyChain::TDoubleLinkedListNode* CurrentNode)
{
	if (!CurrentNode) return;
	const bool IsLootMinQuantity = CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(FLootEntry, MinQuantity);
	const bool IsLootMaxQuantity = CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(FLootEntry, MaxQuantity);
	if (IsLootMinQuantity && MaxQuantity < MinQuantity) MaxQuantity = MinQuantity;
	if (IsLootMaxQuantity && MinQuantity > MaxQuantity) MinQuantity = MaxQuantity; 
}
#endif
