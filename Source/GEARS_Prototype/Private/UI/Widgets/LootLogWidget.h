// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootLogWidget.generated.h"

class UItemEntryWidget;
struct FItemStack;
class FInventoryContainer;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API ULootLogWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
private:
	struct FLogEntry
	{
		TObjectPtr<UItemEntryWidget> Widget;
		int32 Amount;
		FTimerHandle Timer;
	};
	
	void OnItemGained(const FItemStack& ItemStack);
	TObjectPtr<UItemEntryWidget> CreateEntryWidget(const FItemStack& ItemStack);
	void ClearLog(int32 Key);
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UPanelWidget> LogsPanel;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	TSubclassOf<UItemEntryWidget> EntryClass;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true, ClampMin=1, UIMax=50))
	int32 MaxLog = 1;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true, ClampMin=0.1, ClampMax=10))
	float LogDuration = 3;
	
	TMap<int32, FLogEntry> LogsMap;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UItemEntryWidget>> EntryCached;
	
	FInventoryContainer* Inventory;
};
