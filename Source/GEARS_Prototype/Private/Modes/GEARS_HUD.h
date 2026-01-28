// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GEARS_HUD.generated.h"

class UHUDWidget;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API AGEARS_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	const TObjectPtr<UHUDWidget>& GetHUDWidget() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = true))
	TSoftClassPtr<UHUDWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;
};
