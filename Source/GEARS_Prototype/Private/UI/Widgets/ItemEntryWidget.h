// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemEntryWidget.generated.h"

struct FItemDefinition;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UItemEntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetItem(const FItemDefinition& Item, int32 Amount);
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UImage> Icon_Image;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> Amount_TextBlock;
};
