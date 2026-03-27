#pragma once
#include "GameplayTagContainer.h"

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
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
};
