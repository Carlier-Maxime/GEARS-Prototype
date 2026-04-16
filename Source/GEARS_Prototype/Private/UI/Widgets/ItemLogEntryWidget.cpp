// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLogEntryWidget.h"

void UItemLogEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RefreshHandle = OnRefresh.AddLambda([this]
	{
		if (GetVisibility() == ESlateVisibility::Visible) return;
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(FadeAnim);
	});
	OnFadeOutFinish.BindDynamic(this, &ThisClass::OnFadeOutFinished);
}

void UItemLogEntryWidget::NativeDestruct()
{
	OnRefresh.Remove(RefreshHandle);
	Super::NativeDestruct();
}

void UItemLogEntryWidget::RefreshAmount()
{
	Super::RefreshAmount();
	PlayAnimation(UpdateAmountAnim);
	ResetTimer();
}

void UItemLogEntryWidget::ResetTimer()
{
	ClearTimer();
	GetWorld()->GetTimerManager().SetTimer(Timer, [this]
	{
		InFadeOut = true;
		if (FadeAnim)
		{
			PlayAnimation(FadeAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
			BindToAnimationFinished(FadeAnim, OnFadeOutFinish);
		}
		else OnFadeOutFinished();
	}, TimeLeft, false);
	SetVisibility(ESlateVisibility::Visible);
}

void UItemLogEntryWidget::OnFadeOutFinished()
{
	if (!InFadeOut) return;
	SetVisibility(ESlateVisibility::Collapsed);
	OnLogExpired.Broadcast(this);
	InFadeOut = false;
}
