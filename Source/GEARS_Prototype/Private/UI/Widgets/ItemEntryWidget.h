// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Definitions/GEARS_Delegates.h"
#include "ItemEntryWidget.generated.h"

struct FItemStack;
struct FItemDefinition;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UItemEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	void SetItem(const FItemStack& ItemStack);
	void AddAmount(int32 Value);
	FORCEINLINE void RemoveAmount(const int32 Value) { AddAmount(-Value); }
	FORCEINLINE int32 GetItemID() const { return ItemID; }
	
	FOnVoidChanged OnRefresh;
	
protected:
	virtual void RefreshAmount()
	{
		RefreshAmount_Internal();
		OnRefresh.Broadcast();
	}
	virtual void RefreshIcon()
	{
		RefreshIcon_Internal();
		OnRefresh.Broadcast();
	}
	virtual void RefreshAll();

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UImage> Icon_Image;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> Amount_TextBlock;
	
	const FItemDefinition* Item;
	int32 Amount, ItemID;
	
	void RefreshAmount_Internal() const;
	void RefreshIcon_Internal() const;
};
