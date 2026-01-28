// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StyleData.h"
#include "SliderStyleData.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API USliderStyleData : public UStyleData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Style")
	FSliderStyle Slate;
};
