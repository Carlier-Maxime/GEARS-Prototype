// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Inventory/ItemStack.h"
#include "UObject/Object.h"
#include "ItemEntryPayload.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UItemEntryPayload : public UObject
{
	GENERATED_BODY()
	
public:
	const FItemStack* Stack;
};
