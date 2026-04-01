#include "Data/Generation/Loot/LootPool.h"

void FLootPool::Rolls(FInventoryContainer& OutLoot, const FRandomStream& InStream)
{
	if (IsDirty) CleanDirtyFlag();
	if (bRollEachEntry)
	{
		for (auto& Entry : Entries) Entry.Roll(OutLoot, InStream);
		return;
	}
	TArray<int32, TInlineAllocator<16>> EntryRolled;
	EntryRolled.Reserve(RollCount);
	float Weights = bIgnoreWeights ? Entries.Num() : TotalWeights;
	for (auto i=0; i<RollCount; ++i)
	{
		const float Roll = InStream.FRandRange(0.0f, Weights);
		float Accumulated = 0.0f;
		for (auto j=0; j<Entries.Num(); ++j)
		{
			const auto& Entry = Entries[j];
			if (!bAllowDuplicates && EntryRolled.Contains(j)) continue;
			Accumulated += bIgnoreWeights ? 1 : Entry.Weight;
			if (Roll > Accumulated) continue;
			Entry.Roll(OutLoot, InStream);
			if (!bAllowDuplicates)
			{
				EntryRolled.Emplace(j);
				Weights -= bIgnoreWeights ? 1 : Entry.Weight;
			}
			break;
		}
	}
}

void FLootPool::CleanDirtyFlag()
{
	FScopeLock Lock(PoolLock.Get());
	if (!IsDirty) return;
	CalculateWeights();
	IsDirty = false;
}

void FLootPool::CalculateWeights()
{
	TotalWeights = 0;
	for (const auto& Entry : Entries) TotalWeights += Entry.Weight;
}

#if WITH_EDITOR
void FLootPool::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent,
	FEditPropertyChain::TDoubleLinkedListNode* CurrentNode)
{
	if (!CurrentNode) return;
	if (CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(FLootPool, Entries))
	{
		MarkDirty();
		auto Index = PropertyChangedEvent.GetArrayIndex(CurrentNode->GetValue()->GetName());
		if (Index == INDEX_NONE) return;
		Entries[Index].PostEditChangeChainProperty(PropertyChangedEvent, CurrentNode->GetNextNode());
	}
}
#endif
