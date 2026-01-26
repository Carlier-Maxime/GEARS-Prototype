// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Slate/SlateBrushAsset.h"
#include "BorderStyle.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UBorderStyle : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Style")
	TObjectPtr<USlateBrushAsset> BrushAsset = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Style")
	FLinearColor BrushColor = FLinearColor::Gray;
};
