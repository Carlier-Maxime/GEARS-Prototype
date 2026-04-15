// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemEntryWidget.generated.h"

struct FItemStack;
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
	void SetItem(const FItemStack& ItemStack);
	void AddAmount(int32 Value);
	FORCEINLINE void RemoveAmount(const int32 Value) { AddAmount(-Value); }
	
protected:
	void RefreshAmount() const;
	void RefreshIcon() const;
	void RefreshAll() const;
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UImage> Icon_Image;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> Amount_TextBlock;
	
	const FItemDefinition* Item;
	int32 Amount;
};
