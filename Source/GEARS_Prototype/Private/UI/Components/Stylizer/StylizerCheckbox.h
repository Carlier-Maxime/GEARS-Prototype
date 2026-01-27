// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StylizerBase.h"
#include "UI/Styles/CheckboxStyleData.h"
#include "StylizerCheckbox.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UStylizerCheckbox : public UStylizerBase
{
	GENERATED_BODY()
protected:
	virtual void ApplyStyle() override;
	virtual UStyleData* GetStyleAsset() const override;

private:
	UPROPERTY(EditAnywhere, Category="Stylizer", meta=(AllowPrivateAccess = true))
	TObjectPtr<UCheckboxStyleData> Style;
};
