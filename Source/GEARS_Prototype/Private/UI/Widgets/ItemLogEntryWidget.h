// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEntryWidget.h"
#include "ItemLogEntryWidget.generated.h"

class UItemLogEntryWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemLogEntryChanged, TObjectPtr<UItemLogEntryWidget>);

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UItemLogEntryWidget : public UItemEntryWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetTimeLeft(const float Time)
	{
		TimeLeft = Time;
		ResetTimer();
	}
	
	FOnItemLogEntryChanged OnLogExpired;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void RefreshAmount() override;
	void ResetTimer();
	void ClearTimer()
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}

private:
	FTimerHandle Timer;
	float TimeLeft = 1;
};
