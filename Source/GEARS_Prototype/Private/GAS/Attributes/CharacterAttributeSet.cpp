// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"

#include "GameplayTags/GEARS_GameplayTags.h"
#include "Settings/GridParams.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	const auto CellSize = GridParams::Get().GetCellSize();
	InitArrivalPrecision(CellSize * 0.25f);
	InitHarvestRange(CellSize * 1.5f);
	InitRemoteAccessRange(CellSize * 16.f);
	InitHarvestRate(1);
	InitHarvestPower(1);
}

FGameplayAttribute UCharacterAttributeSet::GetInteractionRangeAttribute(const FGameplayTag TypeTag)
{
	if (TypeTag.MatchesTag(TAG_Biome)) return GetArrivalPrecisionAttribute();
	if (TypeTag.MatchesTag(TAG_Resource)) return GetHarvestRangeAttribute();
	return GetArrivalPrecisionAttribute();
}
