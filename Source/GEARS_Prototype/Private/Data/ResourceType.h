// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ResourceType.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UResourceType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FGameplayTag ResourceTag;
	
	UPROPERTY(EditAnywhere, Category = "Visuals")
	TObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditAnywhere, Category = "Generation")
	float NoiseThreshold = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Generation")
	float NoiseScale = 0.05f;
};
