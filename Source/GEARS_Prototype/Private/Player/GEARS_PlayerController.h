// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GEARS_PlayerController.generated.h"

class UPathFollowingComponent;
struct FInputActionValue;
class USpringArmComponent;
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
public:
	AGEARS_PlayerController();
	
	void SnapYaw90();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void StopMovement() override;
	void RequestViewReset();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> DefaultIMC;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	TSoftObjectPtr<UInputAction> ClickAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	TSoftObjectPtr<UInputAction> ZoomAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	TSoftObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	TSoftObjectPtr<UInputAction> OrbitModifier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	TSoftObjectPtr<UInputAction> InventoryAction;
	
private:
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	FTSTicker::FDelegateHandle SnapYawDelegate;
	FDelegateHandle CamSnapChangedHandle;
	
	UPROPERTY()
	UPathFollowingComponent* PathFollowingComponent;
	
	void InteractWithWorld();
	void Zoom(const FInputActionValue& Value);
	void HiddenCursor();
	void ShowCursor();
	void DisableOrbitModif();
	void Look(const FInputActionValue& Value);
	void OpenCloseInv();
};
