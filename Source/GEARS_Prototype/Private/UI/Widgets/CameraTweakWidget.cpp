// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTweakWidget.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CheckBox.h"
#include "Player/GEARS_PlayerController.h"
#include "Settings/CameraSettings.h"

void UCameraTweakWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	const auto Pawn = GetOwningPlayerPawn();
	if (!ensure(Pawn)) return;
	SpringArm = Pawn->FindComponentByClass<USpringArmComponent>();
	Camera = Pawn->FindComponentByClass<UCameraComponent>();
	
	LockPitchCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnLockPitchChanged);
	SnapAngleCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnSnapAngleChanged);
}

void UCameraTweakWidget::OnLockPitchChanged(bool bIsChecked)
{
	const auto Settings = GetMutableDefault<UCameraSettings>();
	Settings->bLockPitch = bIsChecked;
}

void UCameraTweakWidget::OnSnapAngleChanged(bool bIsChecked)
{
	const auto Settings = GetMutableDefault<UCameraSettings>();
	Settings->bSnapYaw90 = bIsChecked;
	const auto PC = Cast<AGEARS_PlayerController>(GetOwningPlayer());
	if (!PC) return;
	PC->SnapYaw90();
}
