// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Definitions/ResourceDef.h"
#include "Engine/DataAsset.h"
#include "ResourceType.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UResourceType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
	FResourceDefinition Data;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override
	{
		auto* CurrentNode = PropertyChangedEvent.PropertyChain.GetHead();
		if (CurrentNode->GetValue()->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Data))
			Data.PostEditChangeChainProperty(PropertyChangedEvent, CurrentNode->GetNextNode());
		Super::PostEditChangeChainProperty(PropertyChangedEvent);
	}
#endif
};
