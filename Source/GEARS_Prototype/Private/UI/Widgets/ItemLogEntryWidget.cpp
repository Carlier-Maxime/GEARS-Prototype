// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLogEntryWidget.h"

void UItemLogEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	OnRefresh.AddLambda([this]
	{
		SetVisibility(ESlateVisibility::Visible);
	});
}

void UItemLogEntryWidget::NativeDestruct()
{

	Super::NativeDestruct();
}

void UItemLogEntryWidget::RefreshAmount()
{
	Super::RefreshAmount();
	ResetTimer();
}

void UItemLogEntryWidget::ResetTimer()
{
	ClearTimer();
	GetWorld()->GetTimerManager().SetTimer(Timer, [this]
	{
		SetVisibility(ESlateVisibility::Collapsed);
		OnLogExpired.Broadcast(this);
	}, TimeLeft, false);
	SetVisibility(ESlateVisibility::Visible);
}
