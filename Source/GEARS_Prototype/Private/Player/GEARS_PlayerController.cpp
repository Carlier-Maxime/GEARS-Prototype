// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GEARS_Macro.h"
#include "InputMappingContext.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Settings/CameraSettings.h"

void AGEARS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowCursor();
	
	if (ensureSoftPtr(DefaultIMC))
	{
		const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		check(Subsystem);
		Subsystem->AddMappingContext(DefaultIMC.LoadSynchronous(), InputPriority);
	}
	if (ensureSoftPtr(ClickFX)) UAssetManager::GetStreamableManager().RequestAsyncLoad(ClickFX.ToSoftObjectPath());
}

void AGEARS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	const auto Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (ensureSoftPtr(ClickAction)) Input->BindAction(ClickAction.LoadSynchronous(), ETriggerEvent::Started, this, &ThisClass::MoveToCursor);
	if (ensureSoftPtr(ZoomAction)) Input->BindAction(ZoomAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Zoom);
	if (ensureSoftPtr(OrbitModifier))
	{
		Input->BindAction(OrbitModifier.LoadSynchronous(), ETriggerEvent::Started, this, &ThisClass::HiddenCursor);
		Input->BindAction(OrbitModifier.LoadSynchronous(), ETriggerEvent::Canceled, this, &ThisClass::ShowCursor);
		Input->BindAction(OrbitModifier.LoadSynchronous(), ETriggerEvent::Completed, this, &ThisClass::ShowCursor);
	}
	if (ensureSoftPtr(LookAction)) Input->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Look);
}

void AGEARS_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	SpringArm = aPawn->FindComponentByClass<USpringArmComponent>();
}

void AGEARS_PlayerController::MoveToCursor()
{
	FHitResult Hit;
	if (!GetPawn()) return;
	if (!GetHitResultUnderCursor(ECC_Visibility, true, Hit)) return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.Location);
	ensureSoftPtrOrRet(ClickFX,);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), 
		ClickFX.LoadSynchronous(), 
		Hit.ImpactPoint, 
		Hit.ImpactNormal.Rotation()
	);
}

void AGEARS_PlayerController::Zoom(const FInputActionValue& Value)
{
	if (!SpringArm) return;
	const auto Settings = GetDefault<UCameraSettings>();
	auto Target = Value.Get<float>() * Settings->GetZoomSpeed();
	if (Settings->bInvertZoomAxis) Target *= -1;
	Target += SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = FMath::Clamp(Target, Settings->GetMinZoomDistance(), Settings->GetMaxZoomDistance());
}

void AGEARS_PlayerController::HiddenCursor()
{
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
}

void AGEARS_PlayerController::ShowCursor()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AGEARS_PlayerController::Look(const FInputActionValue& Value)
{
	const auto Direction = Value.Get<FVector2D>();
	const UCameraSettings* Settings = GetDefault<UCameraSettings>();
	float Target = Direction.Y * Settings->RotationSpeed;
	if (Settings->bInvertRotationAxis) Target *= -1;
	Target += SpringArm->GetRelativeRotation().Pitch;
	Target = FMath::Clamp(Target, Settings->MinPitchAngle, Settings->MaxPitchAngle);
	FRotator NewRotation = SpringArm->GetRelativeRotation();
	NewRotation.Pitch = Target;
	SpringArm->SetRelativeRotation(NewRotation);
}
