// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StyleData.h"
#include "ButtonStyleData.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UButtonStyleData : public UStyleData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Style")
	FButtonStyle Slate;
	UPROPERTY(EditDefaultsOnly, Category="Style")
	FLinearColor ColorAndOpacity;
	UPROPERTY(EditDefaultsOnly, Category="Style")
	FLinearColor BackgroundColor;
};
