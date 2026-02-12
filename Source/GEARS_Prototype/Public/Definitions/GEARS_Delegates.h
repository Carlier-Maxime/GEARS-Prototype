#pragma once

#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(FOnVoidChanged)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoolChanged, bool)

DECLARE_DELEGATE_OneParam(FOnInt32ChangedDelegate, int32)