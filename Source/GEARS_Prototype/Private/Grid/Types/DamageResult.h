#pragma once
#include "GameplayTags/GEARS_GameplayTags.h"

namespace DamageResult
{
	enum class EType : uint8
	{
		None,
		Blocked,
		Hit,
		Destroyed
	};
	
	FORCEINLINE FGameplayTag GetCueTag(const EType Type)
	{
		switch (Type)
		{
			case EType::Hit: return TAG_Cue_Damage_Hit;
			case EType::Destroyed: return TAG_Cue_Damage_Destroyed;
			default: return TAG_Cue_Damage;
		}
	}
}
