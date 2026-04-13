#pragma once
#include "LootEntry.h"

#include "LootPool.generated.h"

USTRUCT()
struct FLootPool
{
	GENERATED_BODY()
	FLootPool() { PoolLock = MakeShared<FCriticalSection>();}
	
	UPROPERTY(EditAnywhere, Category = "Loot")
	FName Name = "LootPool";
	
	UPROPERTY(EditAnywhere, Category = "Loot")
	TArray<FLootEntry> Entries;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = 1, EditCondition = "!bRollEachEntry"))
	uint8 RollCount = 1;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta = (EditCondition = "!bRollEachEntry", ToolTip = "Same entry can be multiple roll during roll this pool"))
	bool bAllowDuplicates = false;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta = (EditCondition = "!bRollEachEntry", ToolTip = "If true, all entries will be rolled regardless of their weight. (same as all entry are same weight)"))
	bool bIgnoreWeights = false;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta = (ToolTip = "If, true, Ignore RollCount and roll each entry"))
	bool bRollEachEntry = false;
	
	FORCEINLINE void MarkDirty() const {IsDirty = true;}
	void Rolls(FInventoryContainer& OutLoot, const FRandomStream& InStream) const;
	
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode);
#endif
	
private:
	void CleanDirtyFlag() const;
	void CalculateWeights() const;
	
	TSharedPtr<FCriticalSection> PoolLock;
	mutable bool IsDirty = true;
	mutable float TotalWeights = 0;
};