#include "IKSlot.h"

bool FIKSlot::operator==(const FIKSlot& Other) const
{
	return Tag == Other.Tag && Weight == Other.Weight && Target.Equals(Other.Target);
}
