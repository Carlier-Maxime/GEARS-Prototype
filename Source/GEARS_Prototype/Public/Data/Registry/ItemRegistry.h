#pragma once
#include "DataRegistry.h"
#include "Data/Wrappers/ItemType.h"

class FItemRegistry : public TDataRegistry<FItemDefinition, UItemType, int16>
{
public:
	explicit FItemRegistry(const FName& Path) : TDataRegistry("ItemRegistry", Path) {}
	explicit FItemRegistry(const TArray<TSoftObjectPtr<UItemType>>& InWrappedData) :
			TDataRegistry("ItemRegistry", InWrappedData)
	{}
};