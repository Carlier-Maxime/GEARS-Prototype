// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_Character.h"
#include "EnhancedInputComponent.h"
#include "GEARS_Macro.h"
#include "GameFramework/CharacterMovementComponent.h"


AGEARS_Character::AGEARS_Character()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false; 
	SpringArm->bDoCollisionTest = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void AGEARS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGEARS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(Input);
	
	ensureSoftPtrOrRet(MoveAction,);
	Input->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AGEARS_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGEARS_Character::Move(const FInputActionValue& Value)
{
	if (const auto PC = GetController()) PC->StopMovement();
	const auto Direction = Value.Get<FVector2D>();
	AddMovementInput(FVector::ForwardVector, Direction.X);
	AddMovementInput(FVector::RightVector, Direction.Y);
}
