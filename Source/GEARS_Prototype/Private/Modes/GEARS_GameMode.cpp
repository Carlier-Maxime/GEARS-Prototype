// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_GameMode.h"

#include "Grid/GridSubsystem.h"

void AGEARS_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	Seed = static_cast<int32>(FDateTime::Now().GetTicks() % INT_MAX);
	UE_LOG(LogTemp, Display, TEXT("Seed: %d"), Seed);
	if (!GetWorld()) return;
	if (const auto Grid = GetWorld()->GetSubsystem<UGridSubsystem>()) Grid->GenWorld(Seed);
}
