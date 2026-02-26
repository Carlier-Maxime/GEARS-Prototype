// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GEARS_Character.h"
#include "Definitions/GEARS_Macro.h"
#include "InputMappingContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/NavigationSystemBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Settings/CameraParams.h"
#include "Settings/CameraSettings.h"
#include "Settings/GridParams.h"

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
	CamSnapChangedHandle = CameraParams::Get().OnSnapYawStateChanged.AddLambda([this](bool) { SnapYaw90(); });
}

void AGEARS_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CameraParams::Get().OnSnapYawStateChanged.Remove(CamSnapChangedHandle);
	Super::EndPlay(EndPlayReason);
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
		Input->BindAction(OrbitModifier.LoadSynchronous(), ETriggerEvent::Canceled, this, &ThisClass::DisableOrbitModif);
		Input->BindAction(OrbitModifier.LoadSynchronous(), ETriggerEvent::Completed, this, &ThisClass::DisableOrbitModif);
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
	if (!GetHitResultUnderCursor(ECC_Visibility, true, Hit)) return;

	MoveToLocation(Hit.Location);
	
	ensureSoftPtrOrRet(ClickFX,);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), 
		ClickFX.LoadSynchronous(), 
		Hit.ImpactPoint, 
		Hit.ImpactNormal.Rotation()
	);
}

void AGEARS_PlayerController::MoveToLocation(FVector Location)
{
	if (!GetPawn()) return;
	auto* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	auto* NavInvoker = GetPawn()->FindComponentByClass<UNavigationInvokerComponent>();
	if (!NavSys || !NavInvoker) return;
	auto Radius = NavInvoker->GetGenerationRadius();
	auto StartLoc = GetPawn()->GetActorLocation();
	auto DistToLoc = FVector::Dist(Location, StartLoc);
	double ExtentVal = GridParams::Get().GetCellSize() * 0.25f;
	if (DistToLoc > Radius) ExtentVal += DistToLoc - Radius;
	FVector Extent(ExtentVal, ExtentVal, ExtentVal);
	FNavLocation NavLoc;
	NavSys->ProjectPointToNavigation(Location, NavLoc, Extent);
	MoveDelayedHandle.Invalidate();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NavLoc.Location);
	if (NavLoc.Location == Location) return;
	const auto* MoveComponent = GetPawn()->GetMovementComponent();
	if (!MoveComponent) return;
	const auto ElapsedHalfTime = 0.5*FVector::Dist(NavLoc.Location, StartLoc) / MoveComponent->GetMaxSpeed();
	if (ElapsedHalfTime < 0.5) return;
	GetWorldTimerManager().SetTimer(MoveDelayedHandle, [this, Location]()
	{
		MoveToLocation(Location);
	}, ElapsedHalfTime, false);
}

void AGEARS_PlayerController::Zoom(const FInputActionValue& Value)
{
	if (!SpringArm) return;
	auto Target = Value.Get<float>() * CameraParams::Get().GetZoomSpeed(SpringArm->TargetArmLength);
	if (CameraParams::Get().IsInvertZoomAxis()) Target *= -1;
	Target += SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = FMath::Clamp(Target, CameraParams::Get().GetMinZoomDistance(), CameraParams::Get().GetMaxZoomDistance());
}

void AGEARS_PlayerController::HiddenCursor()
{
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);
}

void AGEARS_PlayerController::ShowCursor()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture); 
	InputMode.SetHideCursorDuringCapture(true);
	SetInputMode(InputMode);
}

void AGEARS_PlayerController::SnapYaw90()
{
	auto& Ticker = FTSTicker::GetCoreTicker();
	Ticker.RemoveTicker(SnapYawDelegate);
	const auto Settings = GetDefault<UCameraSettings>();
	if (!CameraParams::Get().IsSnapYaw90()) return;
	
	const auto StartRot = SpringArm->GetRelativeRotation();
	auto EndRot = StartRot;
	EndRot.Yaw = FMath::RoundToInt(StartRot.Yaw / 90.f) * 90.f;
	
	const auto StartTime = GetWorld()->GetTimeSeconds();
	const auto Duration = Settings->SnapYawDuration;
	TWeakObjectPtr WeakThis(this);
	SnapYawDelegate = Ticker.AddTicker(FTickerDelegate::CreateLambda([WeakThis, StartTime, StartRot, EndRot, Duration](float){
		if (!WeakThis.IsValid()) return false;
		const auto This = WeakThis.Get();
		const auto ElapsedTime = This->GetWorld()->GetTimeSeconds() - StartTime;
		const auto Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.f);
		This->SpringArm->SetRelativeRotation(FMath::InterpEaseOut(StartRot, EndRot, Alpha, 2));
		return Alpha < 1.f;
	}));
}

void AGEARS_PlayerController::DisableOrbitModif()
{
	ShowCursor();
	SnapYaw90();
}

void AGEARS_PlayerController::Look(const FInputActionValue& Value)
{
	const auto Direction = Value.Get<FVector2D>();
	const UCameraSettings* Settings = GetDefault<UCameraSettings>();
	
	const auto Pitch = SpringArm->GetRelativeRotation().Pitch;
	float TargetPitch;
	if (!CameraParams::Get().IsLockPitch())
	{
		TargetPitch = Direction.Y * CameraParams::Get().GetPitchSpeed(Pitch);
		if (Settings->bInvertPitchAxis) TargetPitch *= -1;
		TargetPitch += Pitch;
		TargetPitch = FMath::Clamp(TargetPitch, CameraParams::Get().GetMinPitch(), CameraParams::Get().GetMaxPitch());
	} else TargetPitch = Pitch;
	
	if (CameraParams::Get().IsSnapYaw90()) FTSTicker::GetCoreTicker().RemoveTicker(SnapYawDelegate);
	const auto Yaw = SpringArm->GetRelativeRotation().Yaw;
	float TargetYaw = Direction.X * CameraParams::Get().GetYawSpeed(Yaw);
	if (Settings->bInvertYawAxis) TargetYaw *= -1;
	TargetYaw += Yaw;
	
	const FRotator NewRotation(TargetPitch, TargetYaw, 0.f);
	SpringArm->SetRelativeRotation(NewRotation);
}

void AGEARS_PlayerController::RequestViewReset()
{
	const auto MyChar = Cast<AGEARS_Character>(GetCharacter());
	if (!MyChar) return;
	MyChar->ResetView();
}

void AGEARS_PlayerController::StopMovement()
{
	MoveDelayedHandle.Invalidate();
	Super::StopMovement();
}
