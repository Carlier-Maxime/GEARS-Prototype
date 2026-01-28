// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UCameraTweakWidget;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCameraTweakWidget> CameraTweak;
};
