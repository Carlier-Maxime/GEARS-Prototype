// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraTweakWidget.generated.h"

class UTextBlock;
class USlider;
class UButton;
class UCheckBox;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UCameraTweakWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnLockPitchChanged(bool bIsChecked);
	UFUNCTION()
	void OnSnapAngleChanged(bool bIsChecked);
	UFUNCTION()
	void OnLockPitchUpdated(bool bValue);
	UFUNCTION()
	void OnSnapAngleUpdated(bool bValue);
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCheckBox> LockPitchCheckBox;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCheckBox> SnapAngleCheckBox;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> ResetViewButton;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<USlider> FOVSlider;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> FOVTextBlock;
	
	UPROPERTY(Transient)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(Transient)
	TObjectPtr<UCameraComponent> Camera;
};
