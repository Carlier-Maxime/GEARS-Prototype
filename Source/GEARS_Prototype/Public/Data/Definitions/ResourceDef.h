#pragma once
#include "GameplayTagContainer.h"
#include "Data/Generation/PlacementRule.h"
#include "Data/Generation/Loot/LootRule.h"
#include "Data/States/ResourceState.h"

#include "ResourceDef.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FResourceDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	FGameplayTag Tag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sampling")
	FPlacementRule PlacementRule;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	FLootRule Loot;
	
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FResourceState State;
	
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent, FEditPropertyChain::TDoubleLinkedListNode* CurrentNode)
	{
		if (!CurrentNode) return;
		if (CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(FResourceDefinition, Loot))
			Loot.PostEditChangeChainProperty(PropertyChangedEvent, CurrentNode->GetNextNode());
	}
#endif
};