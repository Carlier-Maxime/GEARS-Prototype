// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BiomeType.generated.h"

class UResourceType;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UBiomeType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Identity")
	FGameplayTag BiomeTag;

	UPROPERTY(EditDefaultsOnly, Category="Generation")
	FFloatRange Temperature;
	UPROPERTY(EditDefaultsOnly, Category="Generation")
	FFloatRange Humidity;
	
	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TArray<TSoftObjectPtr<UResourceType>> Resources;
	
	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	FLinearColor Color;
};
