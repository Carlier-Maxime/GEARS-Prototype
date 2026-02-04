// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTweakWidget.h"

#include "Camera/CameraComponent.h"
#include "Components/Button.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Player/GEARS_PlayerController.h"
#include "Settings/CameraParams.h"

void UCameraTweakWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	const auto Pawn = GetOwningPlayerPawn();
	if (!ensure(Pawn)) return;
	SpringArm = Pawn->FindComponentByClass<USpringArmComponent>();
	Camera = Pawn->FindComponentByClass<UCameraComponent>();
	
	LockPitchCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnLockPitchChanged);
	SnapAngleCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnSnapAngleChanged);
	
	CameraParams::Get().OnLockPitchChanged.AddUObject(this, &ThisClass::OnLockPitchUpdated);
	CameraParams::Get().OnSnapYawStateChanged.AddUObject(this, &ThisClass::OnSnapAngleUpdated);
	OnLockPitchUpdated(CameraParams::Get().IsLockPitch());
	OnSnapAngleUpdated(CameraParams::Get().IsSnapYaw90());
	
	FOVSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnFOVChanged);
	FOVSlider->SetMinValue(CameraParams::Get().GetMinFOV());
	FOVSlider->SetMaxValue(CameraParams::Get().GetMaxFOV());
	FOVSlider->SetValue(Camera->FieldOfView);
	OnFOVChanged(Camera->FieldOfView);
	
	ResetViewButton->OnClicked.AddDynamic(this, &ThisClass::ResetView);
}

void UCameraTweakWidget::NativeDestruct()
{
	CameraParams::Get().OnLockPitchChanged.RemoveAll(this);
	CameraParams::Get().OnSnapYawStateChanged.RemoveAll(this);
	Super::NativeDestruct();
}

void UCameraTweakWidget::OnLockPitchChanged(bool bIsChecked)
{
	CameraParams::Get().SetLockPitch(bIsChecked);
}

void UCameraTweakWidget::OnSnapAngleChanged(bool bIsChecked)
{
	CameraParams::Get().SetSnapYaw90(bIsChecked);
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

void UCameraTweakWidget::OnFOVChanged(float Value)
{
	static const auto FOVFormatOptions = []{
		FNumberFormattingOptions Options;
		Options.MinimumIntegralDigits = 2;
		Options.MaximumFractionalDigits = 0;
		Options.RoundingMode = HalfToEven;
		return Options;
	}();
	
	Camera->FieldOfView = Value;
	FOVTextBlock->SetText(FText::AsNumber(Value, &FOVFormatOptions));
	FOVTextBlock->SetToolTipText(FText::AsNumber(Value));
}

void UCameraTweakWidget::ResetView()
{
	const auto PC = Cast<AGEARS_PlayerController>(GetOwningPlayer());
	if (!PC) return;
	PC->RequestViewReset();
	OnFOVChanged(Camera->FieldOfView);
}
