// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationInvokerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/Attributes/CharacterAttributeSet.h"
#include "GEARS_Character.generated.h"

class UInputAction;

UCLASS()
class GEARS_PROTOTYPE_API AGEARS_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGEARS_Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void ResetView();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNavigationInvokerComponent> NavInvoker;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation", meta = (AllowPrivateAccess = true, ClampMin=1, UIMax=8))
	uint8 NavGenerationMultiplier = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation", meta = (AllowPrivateAccess = true, ClampMin=1, UIMax=16))
	uint8 NavRemovalMultiplier = 4;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = true))
	USpringArmComponent *SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = true))
	UCameraComponent *Camera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action", meta=(AllowPrivateAccess = true))
	TSoftObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY()
	UAbilitySystemComponent* ASC;
	UPROPERTY()
	const UCharacterAttributeSet* AttributeSet;
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	
	void MoveStart();
	void Move(const struct FInputActionValue& Value);
	void MoveEnd();
	void AutoSetNavRadius();
};
