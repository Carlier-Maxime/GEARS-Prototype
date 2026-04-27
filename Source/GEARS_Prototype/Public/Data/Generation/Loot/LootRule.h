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
	UPROPERTY(EditDefaultsOnly, Category = "Loot", meta = (EditCondition = bRollOnDestroy, ClampMin=0, ClampMax=1, ToolTip = "The multiplicator / ratio of the loot rolled when roll on destroy."))
	float FinalRatio = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	bool bRollPerDamage = false;
	UPROPERTY(EditDefaultsOnly, Category = "Loot", meta = (EditCondition = bRollPerDamage, ClampMin=1, UIMax=100))
	float DamageRequiredPerRoll = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Loot", meta = (EditCondition = bRollPerDamage, ClampMin=0, ClampMax=1, ToolTip = "The multiplicator / ratio of the loot rolled when roll per damage."))
	float IntermediateRatio = 0.7f;
	
	void RollsFromDamage(float& AccDamage, FInventoryContainer& OutLoot, const FRandomStream& InStream, float Ratio) const;
	
	FORCEINLINE void TryRollOnDestroy(FInventoryContainer& OutLoot, const FRandomStream& InStream, float Ratio) const
	{
		if (bRollOnDestroy) Table.GenerateLoot(OutLoot, InStream, FinalRatio * Ratio);
	}
	
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode);
#endif
};
