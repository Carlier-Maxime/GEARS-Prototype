// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StyleData.h"
#include "Slate/SlateBrushAsset.h"
#include "BorderStyle.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UBorderStyle : public UStyleData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Style")
	TObjectPtr<USlateBrushAsset> BrushAsset = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Style")
	FLinearColor BrushColor = FLinearColor::Gray;
};
