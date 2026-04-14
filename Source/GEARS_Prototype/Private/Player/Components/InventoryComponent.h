// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inventory/InventoryContainer.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEARS_PROTOTYPE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	FORCEINLINE FInventoryContainer& GetInventory() { return Inventory; }

protected:
	virtual void BeginPlay() override;
	
private:
	FInventoryContainer Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartedCapacity = 0;
};
