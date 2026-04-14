#pragma once
#include "GameplayTagContainer.h"
#include "Data/Inventory/ItemVisualThreshold.h"

#include "ItemDefinition.generated.h"

USTRUCT()
struct FItemDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	FGameplayTag Tag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Logic")
	int32 StackSize = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FItemVisual DefaultVisual;
	
	UPROPERTY(EditDefaultsOnly, Category= "Visuals")
	TArray<FItemVisualThreshold> VisualThresholds;
	
	const FItemVisual& GetVisual(int32 Quantity) const
	{
		for (int32 i=VisualThresholds.Num()-1; i>=0; --i)
		{
			const auto& Threshold = VisualThresholds[i];
			if (Quantity >= Threshold.MinQuantity)
				return Threshold.Visual;
		}
		return DefaultVisual;
	}
};
