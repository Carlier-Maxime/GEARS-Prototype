// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GEARS_Macro.h"
#include "InputMappingContext.h"

void AGEARS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ensureSoftPtrOrRet(DefaultIMC,);
	const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(DefaultIMC.LoadSynchronous(), InputPriority);
}
