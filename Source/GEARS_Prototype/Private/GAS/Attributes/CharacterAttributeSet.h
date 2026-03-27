// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Interaction")
	FGameplayAttributeData ArrivalPrecision;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ArrivalPrecision);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Interaction")
	FGameplayAttributeData HarvestRange;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HarvestRange);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Interaction")
	FGameplayAttributeData RemoteAccessRange;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, RemoteAccessRange);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Harvest")
	FGameplayAttributeData HarvestRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HarvestRate)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Harvest")
	FGameplayAttributeData HarvestPower;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HarvestPower)
	
	static FGameplayAttribute GetInteractionRangeAttribute(FGameplayTag TypeTag); 
};
