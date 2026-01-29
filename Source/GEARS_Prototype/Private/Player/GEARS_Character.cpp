// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_Character.h"
#include "EnhancedInputComponent.h"
#include "Definitions/GEARS_Macro.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/CameraSettings.h"


AGEARS_Character::AGEARS_Character()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	ResetView();
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
	
	const FRotator Rotation = SpringArm->GetRelativeRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, Direction.X);
	AddMovementInput(RightDirection, Direction.Y);
}

void AGEARS_Character::ResetView()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArm->TargetArmLength = GetDefault<UCameraSettings>()->GetDefaultZoomDistance();
	SpringArm->SetRelativeRotation(FRotator(GetDefault<UCameraSettings>()->DefaultPitchAngle, 0.f, 0.f));
	
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false; 
	SpringArm->bDoCollisionTest = false;
	
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = GetDefault<UCameraSettings>()->GetDefaultFOV();
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}
