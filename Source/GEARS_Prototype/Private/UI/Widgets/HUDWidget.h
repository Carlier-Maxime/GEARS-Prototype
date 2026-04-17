// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class FInventoryContainer;
class ULootLogWidget;
class UCameraTweakWidget;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	static FInventoryContainer* GetInventory(APawn* Pawn);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCameraTweakWidget> CameraTweak;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<ULootLogWidget> LootLog;
};
