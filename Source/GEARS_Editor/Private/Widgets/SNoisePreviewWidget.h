// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Preview/NoisePreviewState.h"
#include "Widgets/SCompoundWidget.h"
#include "PropertyHandle.h"
#include "Preview/NoisePreviewSettings.h"

/**
 * 
 */
class GEARS_EDITOR_API SNoisePreviewWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNoisePreviewWidget) :
			_ThumbnailSize(128), _StructPtr(nullptr), _StructName("Unknown"), _PropertyHandle(nullptr) {}
		SLATE_ARGUMENT(int32, ThumbnailSize)
		SLATE_ARGUMENT(void*, StructPtr)
		SLATE_ARGUMENT(FName, StructName)
		SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, PropertyHandle)
	SLATE_END_ARGS()

	void InitializeSettingsViews();
	void Construct(const FArguments& InArgs);

private:
	static void AddNumericRow(const TSharedRef<SGridPanel>& Grid, const int32 Row, const FName& Label, const TSharedRef<ISinglePropertyView>& PropertyView);
	
	FNoisePreviewState State;
	TSharedPtr<FStructOnScope> SettingsStructOnScope;
	TMap<FName, TSharedPtr<ISinglePropertyView>> SettingsViews;
};
