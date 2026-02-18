#pragma once
#include "DataRegistry.h"
#include "Data/Wrappers/BiomeType.h"

class FBiomeRegistry : public TDataRegistry<FBiomeDefinition, UBiomeType, uint8>
{
public:
	explicit FBiomeRegistry(const TArray<TSoftObjectPtr<UBiomeType>>& InWrappedData) :
			TDataRegistry("BiomeRegistry", InWrappedData)
	{}
};
