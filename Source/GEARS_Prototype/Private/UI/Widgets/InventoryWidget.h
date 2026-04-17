// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class FInventoryContainer;
class UListView;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	void OnSlotChanged(int SlotIndex);
	void OnCapacityChanged(int I);
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	void CompleteMissingSlots();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true, BindWidget))	
	TObjectPtr<UListView> Grid;
	
	FInventoryContainer* Inventory;
};
