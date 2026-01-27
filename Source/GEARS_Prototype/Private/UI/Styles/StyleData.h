// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StyleData.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStyleChanged);

/**
 * 
 */
UCLASS(BlueprintType)
class GEARS_PROTOTYPE_API UStyleData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FOnStyleChanged OnStyleChanged;
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};
