// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UGridSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FIntPoint WorldToGrid(const FVector& WorldPosition);
	FVector GridToWorld(const FIntPoint& GridPosition);
	
protected:
	void UpdateCache();
	
	private:
	float CachedCellSize;
	float CachedInvCellSize;
};
