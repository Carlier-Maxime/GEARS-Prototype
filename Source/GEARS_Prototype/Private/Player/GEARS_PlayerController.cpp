// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GEARS_Macro.h"
#include "InputMappingContext.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/AssetManager.h"

void AGEARS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
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
	ensureSoftPtrOrRet(ClickAction,);
	Input->BindAction(ClickAction.LoadSynchronous(), ETriggerEvent::Started, this, &AGEARS_PlayerController::MoveToCursor);
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
