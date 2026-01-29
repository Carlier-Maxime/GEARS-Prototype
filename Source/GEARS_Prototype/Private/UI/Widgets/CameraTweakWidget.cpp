// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTweakWidget.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CheckBox.h"
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
	
	const auto Settings = GetMutableDefault<UCameraSettings>();
	Settings->OnLockPitchChanged.AddUObject(this, &ThisClass::OnLockPitchUpdated);
	Settings->OnSnapYawStateChanged.AddUObject(this, &ThisClass::OnSnapAngleUpdated);
	OnLockPitchUpdated(Settings->IsLockPitch());
	OnSnapAngleUpdated(Settings->IsSnapYaw90());
}

void UCameraTweakWidget::NativeDestruct()
{
	const auto Settings = GetMutableDefault<UCameraSettings>();
	Settings->OnLockPitchChanged.RemoveAll(this);
	Settings->OnSnapYawStateChanged.RemoveAll(this);
	Super::NativeDestruct();
}

void UCameraTweakWidget::OnLockPitchChanged(bool bIsChecked)
{
	const auto Settings = GetMutableDefault<UCameraSettings>();
	Settings->SetLockPitch(bIsChecked);
}

void UCameraTweakWidget::OnSnapAngleChanged(bool bIsChecked)
{
	const auto Settings = GetMutableDefault<UCameraSettings>();
	Settings->SetSnapYaw90(bIsChecked);
}

void UCameraTweakWidget::OnLockPitchUpdated(bool bValue)
{
	if (LockPitchCheckBox->IsChecked() == bValue) return;
	LockPitchCheckBox->SetIsChecked(bValue);
}

void UCameraTweakWidget::OnSnapAngleUpdated(bool bValue)
{
	if (SnapAngleCheckBox->IsChecked() == bValue) return;
	SnapAngleCheckBox->SetIsChecked(bValue);
}
