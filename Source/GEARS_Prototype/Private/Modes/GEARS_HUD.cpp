// Fill out your copyright notice in the Description page of Project Settings.


#include "GEARS_HUD.h"

#include "GEARS_Macro.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/HUDWidget.h"

void AGEARS_HUD::BeginPlay()
{
	Super::BeginPlay();
	ensureSoftPtrOrRet(HUDWidgetClass,)
	HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass.LoadSynchronous());
	HUDWidget->AddToViewport();
}

const TObjectPtr<UHUDWidget>& AGEARS_HUD::GetHUDWidget() const
{
	return HUDWidget;
}
