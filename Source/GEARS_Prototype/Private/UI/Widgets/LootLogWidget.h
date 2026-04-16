// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootLogWidget.generated.h"

class UItemLogEntryWidget;
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
	void OnItemGained(const FItemStack& ItemStack);
	void OnLogExpired(TObjectPtr<UItemLogEntryWidget> ItemLogEntryWidget);
	TObjectPtr<UItemLogEntryWidget> CreateEntryWidget(const FItemStack& ItemStack);
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UPanelWidget> LogsPanel;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	TSubclassOf<UItemLogEntryWidget> EntryClass;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true, ClampMin=1, UIMax=50))
	int32 MaxLog = 1;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true, ClampMin=0.1, ClampMax=10))
	float LogDuration = 3;
	
	TMap<int32, TObjectPtr<UItemLogEntryWidget>> LogsMap;
	TArray<TObjectPtr<UItemLogEntryWidget>> EntryCached;
	
	FInventoryContainer* Inventory;
};
