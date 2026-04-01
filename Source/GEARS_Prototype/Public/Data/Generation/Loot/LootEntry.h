#pragma once
#include "GameplayTagContainer.h"
#include "GameplayTags/GEARS_GameplayTags.h"

#include "LootEntry.generated.h"

class FInventoryContainer;

USTRUCT()
struct FLootEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (Categories = "Item"))
	FGameplayTag Item = TAG_Item_None;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = 1))
	int32 MinQuantity = 1;

	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = 1))
	int32 MaxQuantity = 1;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = 0.0, ToolTip = "Relative weight of this entry within its pool. Higher weight means higher selection priority."))
	float Weight = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = 0.0, ClampMax = 1.0, ToolTip = "Absolute chance (0.0 to 1.0) to drop if this entry is selected by the pool's weight roll."))
	float DropChance = 1.0f;
	
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode);
#endif
	
	void Roll(FInventoryContainer& OutLoot, const FRandomStream& InStream) const;
};