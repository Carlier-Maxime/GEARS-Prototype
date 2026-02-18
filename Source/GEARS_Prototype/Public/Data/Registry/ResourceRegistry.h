#pragma once
#include "DataRegistry.h"
#include "Data/Wrappers/ResourceType.h"

class FResourceRegistry : public TDataRegistry<FResourceDefinition, UResourceType, int16>
{
public:
	explicit FResourceRegistry(const TArray<TSoftObjectPtr<UResourceType>>& InWrappedData) :
			TDataRegistry("ResourceRegistry", InWrappedData)
	{}
};
