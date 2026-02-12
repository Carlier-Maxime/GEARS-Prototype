// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Preview/NoisePreviewState.h"
#include "Widgets/SCompoundWidget.h"
#include "PropertyHandle.h"
#include "Definitions/GEARS_Delegates.h"

/**
 * 
 */
class GEARS_EDITOR_API SNoisePreviewWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNoisePreviewWidget) :
			_PropertyHandle(nullptr) {}
		SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, PropertyHandle)
		SLATE_EVENT(FOnInt32ChangedDelegate, OnSeedChanged)
		SLATE_EVENT(FOnGenerateColor, OnGenerateColor)
	SLATE_END_ARGS()

	void InitializeSettingsViews();
	void Construct(const FArguments& InArgs);

private:
	static void AddNumericRow(const TSharedRef<SGridPanel>& Grid, const int32 Row, const FName& Label, const TSharedRef<ISinglePropertyView>& PropertyView);
	FColor GetColorAtPos(const FGridPosition& Pos) const;
	
	FOnInt32ChangedDelegate OnSeedChanged;
	FOnGenerateColor OnGenerateColor;
	FNoisePreviewState State;
	TSharedPtr<FStructOnScope> SettingsStructOnScope;
	TMap<FName, TSharedPtr<ISinglePropertyView>> SettingsViews;
};
