// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StylizerBase.h"
#include "UI/Styles/TextBlockStyleData.h"
#include "StylizerTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UStylizerTextBlock : public UStylizerBase
{
	GENERATED_BODY()
protected:
	virtual void ApplyStyle() override;
	virtual const UDataAsset* GetStyleAsset() const override;

private:
	UPROPERTY(EditAnywhere, Category="Stylizer", meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextBlockStyleData> Style;
};
