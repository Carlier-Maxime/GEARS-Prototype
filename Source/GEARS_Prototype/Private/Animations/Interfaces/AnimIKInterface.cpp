#include "AnimIKInterface.h"

#include "GameplayTags/GEARS_GameplayTags.h"

FIKSlot& IAnimIKInterface::GetIK(const FGameplayTag Tag)
{
	
	auto* IK = IKSlots.Find(Tag);
	if (IK) return *IK;
	if (!Tag.MatchesTag(TAG_IK)) UE_LOG(LogTemp, Error, TEXT("Invalid IK tag: %s"), *Tag.ToString());
	FIKSlot NewIK;
	NewIK.Tag = Tag;
	NewIK.Weight = 0;
	return IKSlots.Add(Tag, NewIK);
}
