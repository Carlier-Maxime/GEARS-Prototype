#pragma once
#include "Data/Definitions/ItemDefinition.h"

#include "ItemType.generated.h"

UCLASS()
class GEARS_PROTOTYPE_API UItemType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FItemDefinition Data;
};