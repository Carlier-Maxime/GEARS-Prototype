// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Definitions/BiomeDef.h"
#include "Engine/DataAsset.h"
#include "BiomeType.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UBiomeType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FBiomeDefinition Data;
};
