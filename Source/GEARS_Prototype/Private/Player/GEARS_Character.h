// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GEARS_Character.generated.h"

class UInputAction;

UCLASS()
class GEARS_PROTOTYPE_API AGEARS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	AGEARS_Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = true))
	USpringArmComponent *SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = true))
	UCameraComponent *Camera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action", meta=(AllowPrivateAccess = true))
	TSoftObjectPtr<UInputAction> MoveAction;
	
	void Move(const struct FInputActionValue& Value);
};
