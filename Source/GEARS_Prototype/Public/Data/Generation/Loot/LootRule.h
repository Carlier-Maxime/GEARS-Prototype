#pragma once
#include "LootTable.h"

#include "LootRule.generated.h"

USTRUCT()
struct FLootRule
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	FLootTable Table;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	bool bRollOnDestroy = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	bool bRollPerDamage = false;
	UPROPERTY(EditDefaultsOnly, Category = "Loot", meta = (EditCondition = bRollPerDamage, ClampMin=1, UIMax=100))
	float DamageRequiredPerRoll = 1;
	
	void RollsFromDamage(float& AccDamage, FInventoryContainer& OutLoot, const FRandomStream& InStream) const;
	
	FORCEINLINE void TryRollOnDestroy(FInventoryContainer& OutLoot, const FRandomStream& InStream) const
	{
		if (bRollOnDestroy) Table.GenerateLoot(OutLoot, InStream);
	}
	
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode);
#endif
};
