#pragma once
#include "LootPool.h"

#include "LootTable.generated.h"

USTRUCT()
struct FLootTable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FLootPool> Pools;
	
	void GenerateLoot(FInventoryContainer& OutLoot, const FRandomStream& InStream) const;
	void GenerateLoot(FInventoryContainer& OutLoot, const FRandomStream& InStream, float Ratio) const;
	
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode);
#endif
};