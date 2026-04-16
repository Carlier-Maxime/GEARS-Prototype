// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Player/Components/InventoryComponent.h"

FInventoryContainer* UHUDWidget::GetInventory(APawn* Pawn)
{
	if (!ensure(Pawn)) return nullptr;
	auto* InvComp = Pawn->FindComponentByClass<UInventoryComponent>();
	return InvComp ? &InvComp->GetInventory() : nullptr;
}
