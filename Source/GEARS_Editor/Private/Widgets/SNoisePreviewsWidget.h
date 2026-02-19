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
class GEARS_EDITOR_API SNoisePreviewsWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNoisePreviewsWidget) {}
		SLATE_ARGUMENT(TArray<TSharedRef<IPropertyHandle>>, PropertyHandles)
		SLATE_EVENT(FOnInt32ChangedDelegate, OnSeedChanged)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	void AddPreview(const TFunction<FColor(FWorldGridPos)>& GenColorFn);

private:
	static void AddNumericRow(const TSharedRef<SGridPanel>& Grid, const int32 Row, const FName& Label, const TSharedRef<ISinglePropertyView>& PropertyView);
	void InitializeSettingsViews();
	void BindPropertyCallbacks(const TArray<TSharedRef<IPropertyHandle>>& PropertyHandles);
	
	
	FOnInt32ChangedDelegate OnSeedChanged;
	FOnGenerateColor OnGenerateColor;
	TSharedPtr<SHorizontalBox> PreviewsContainer;
	TArray<FNoisePreviewState> Previews;
	FNoisePreviewContext NoiseParams;
	TSharedPtr<FStructOnScope> SettingsStructOnScope;
	TMap<FName, TSharedPtr<ISinglePropertyView>> SettingsViews;
};
