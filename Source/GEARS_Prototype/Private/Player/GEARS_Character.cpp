// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_Character.h"


// Sets default values
AGEARS_Character::AGEARS_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGEARS_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGEARS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGEARS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

