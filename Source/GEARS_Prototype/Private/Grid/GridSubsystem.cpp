// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSubsystem.h"

#include "Settings/GridSettings.h"

void UGridSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UpdateCache();
	const auto Settings = GetMutableDefault<UGridSettings>();
	Settings->OnUpdated.AddUObject(this, &ThisClass::UpdateCache);
}

void UGridSubsystem::Deinitialize()
{
	const auto Settings = GetMutableDefault<UGridSettings>();
	Settings->OnUpdated.RemoveAll(this);
	Super::Deinitialize();
}

void UGridSubsystem::UpdateCache()
{
	const auto Settings = GetDefault<UGridSettings>();
	CachedCellSize = Settings->GetCellSize();
	CachedInvCellSize = Settings->GetInvCellSize();
}

FIntPoint UGridSubsystem::WorldToGrid(const FVector& WorldPosition)
{
	return FIntPoint(
		FMath::FloorToInt(WorldPosition.X * CachedInvCellSize),
		FMath::FloorToInt(WorldPosition.Y * CachedInvCellSize)
	);
}

FVector UGridSubsystem::GridToWorld(const FIntPoint& GridPosition)
{
	return FVector(
		(GridPosition.X + 0.5f) * CachedCellSize,
		(GridPosition.Y + 0.5f) * CachedCellSize,
		0.1f
	);
}
