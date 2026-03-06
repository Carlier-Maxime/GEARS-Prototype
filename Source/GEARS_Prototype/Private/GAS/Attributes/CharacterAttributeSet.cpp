// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"

#include "Settings/GridParams.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	const auto CellSize = GridParams::Get().GetCellSize();
	InitArrivalPrecision(CellSize * 0.25f);
	InitHarvestRange(CellSize * 1.5f);
	InitRemoteAccessRange(CellSize * 16.f);
}
