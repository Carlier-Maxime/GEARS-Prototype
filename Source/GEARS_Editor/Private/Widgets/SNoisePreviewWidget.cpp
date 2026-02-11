// Fill out your copyright notice in the Description page of Project Settings.


#include "SNoisePreviewWidget.h"

#include "SlateOptMacros.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Preview/NoisePreviewCache.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNoisePreviewWidget::Construct(const FArguments& InArgs)
{
	ThumbnailSize = InArgs._ThumbnailSize;
	Cache.StructPtr = InArgs._StructPtr;
	Cache.StructName = InArgs._StructName;
	Cache.Update();
	
	if (const auto Handle = InArgs._PropertyHandle; Handle->IsValidHandle())
	{
		Handle->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda([this]
		{
			Cache.Update();
		}));
	}
	
	TSharedRef<SGridPanel> Grid = SNew(SGridPanel);
	AddNumericRow(Grid, 0, "Seed", &Cache.Seed, 0, 1000);
	AddNumericRow(Grid, 1, "Resolution", &Cache.Resolution, 32, 512, 32);
	AddNumericRow(Grid, 2, "Thumbnail Res", &ThumbnailSize, 64, 256, 32);
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5.f)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(&Cache.Brush)
			.DesiredSizeOverride_Lambda([this]{
				return FVector2D{
					static_cast<double>(ThumbnailSize),
					static_cast<double>(ThumbnailSize)
				};
			}).OnMouseButtonDown_Lambda([this](const FGeometry&, const FPointerEvent&)
			{
				const auto Subsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
				if (!Subsystem) return FReply::Unhandled();
				Subsystem->OpenEditorForAsset(Cache.Texture.Get());
				return FReply::Handled();
			})
		]

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(10, 5)
		[
			Grid
		]
	];
}

void SNoisePreviewWidget::AddNumericRow(
	const TSharedRef<SGridPanel>& Grid, const int32 Row, const FString& Label, int32* ValuePtr,
	int32 Min, int32 Max, const int32 Step
) {
	Grid->AddSlot(0, Row).Padding(0, 5).VAlign(VAlign_Center)
	[
		SNew(STextBlock).Text(FText::FromString(Label)).Font(IDetailLayoutBuilder::GetDetailFont())
	];
	Grid->AddSlot(1, Row).Padding(5, 5)
	[
		SNew(SNumericEntryBox<int32>)
		.MinDesiredValueWidth(128.f)
		.MinSliderValue(Min)
		.MaxSliderValue(Max)
		.Delta(Step)
		.AllowSpin(true)
		.Value_Lambda([ValuePtr] { return ValuePtr ? *ValuePtr : 0; })
		.OnValueChanged_Lambda([this, ValuePtr, Min, Max](int32 Val){ 
			*ValuePtr = FMath::Clamp(Val, Min, Max);
			Cache.Update();
		})
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
