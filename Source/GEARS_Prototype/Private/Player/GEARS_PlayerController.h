// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GEARS_PlayerController.generated.h"

class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API AGEARS_PlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> DefaultIMC;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	TSoftObjectPtr<UInputAction> ClickAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FX")
	TSoftObjectPtr<UNiagaraSystem> ClickFX;
	
private:
	void MoveToCursor();
};
