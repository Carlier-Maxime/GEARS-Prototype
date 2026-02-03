// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GEARS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API AGEARS_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
	int32 Seed;
};
