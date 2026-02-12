// Fill out your copyright notice in the Description page of Project Settings.


#include "SNoisePreviewWidget.h"

#include "SlateOptMacros.h"
#include "DetailLayoutBuilder.h"
#include "ISinglePropertyView.h"
#include "IStructureDataProvider.h"
#include "Grid/Types/GridPosition.h"
#include "Preview/NoisePreviewState.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNoisePreviewWidget::Construct(const FArguments& InArgs)
{
	InitializeSettingsViews();
	OnSeedChanged = InArgs._OnSeedChanged;
	OnGenerateColor = InArgs._OnGenerateColor;
	BindPropertyCallbacks(InArgs._PropertyHandles);
	
	TSharedRef<SGridPanel> Grid = SNew(SGridPanel);
	int32 Row = 0;
	for (auto& [Name, View] : SettingsViews)
	{
		AddNumericRow(Grid, ++Row, Name, View.ToSharedRef());
	}
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5.f)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(&State.Brush)
			.DesiredSizeOverride_Lambda([this]{
				return FVector2D{
					static_cast<double>(State.Settings.ThumbnailSize),
					static_cast<double>(State.Settings.ThumbnailSize)
				};
			}).OnMouseButtonDown_Lambda([this](const FGeometry&, const FPointerEvent&)
			{
				const auto Subsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
				if (!Subsystem) return FReply::Unhandled();
				Subsystem->OpenEditorForAsset(State.Texture.Get());
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
	const TSharedRef<SGridPanel>& Grid, const int32 Row, const FName& Label, const TSharedRef<ISinglePropertyView>& PropertyView
) {
	Grid->AddSlot(0, Row).Padding(0, 5).VAlign(VAlign_Center)
	[
		SNew(STextBlock).Text(FText::FromName(Label)).Font(IDetailLayoutBuilder::GetDetailFont())
	];
	Grid->AddSlot(1, Row).Padding(5, 0)
	[
		SNew(SBox).MinDesiredWidth(128.f)
		[
			PropertyView
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNoisePreviewWidget::InitializeSettingsViews()
{
	SettingsStructOnScope = MakeShared<FStructOnScope>(FNoisePreviewSettings::StaticStruct(), reinterpret_cast<uint8*>(&State.Settings));
	TSharedRef<IStructureDataProvider> StructProvider = MakeShared<FStructOnScopeStructureDataProvider>(SettingsStructOnScope);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;
	
	const UScriptStruct* StructDefinition = FNoisePreviewSettings::StaticStruct();
	for (TFieldIterator<FProperty> It(StructDefinition); It; ++It)
	{
		FProperty* Prop = *It;
		FName PropName = Prop->GetFName();
		
		TSharedPtr<ISinglePropertyView> PropView = PropertyModule.CreateSingleProperty(
			StructProvider, 
			PropName, 
			Params
		);

		if (PropView.IsValid()) SettingsViews.Add(PropName, PropView);
	}
}

void SNoisePreviewWidget::BindPropertyCallbacks(const TArray<TSharedRef<IPropertyHandle>>& PropertyHandles)
{
	for (auto& PropertyHandle : PropertyHandles)
	{
		if (!PropertyHandle->IsValidHandle()) continue;
		PropertyHandle->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda([this]
		{
			State.Update();
		}));
	}
	
	State.OnGenerateColor = FOnGenerateColor::CreateLambda([this](const FGridPosition& Pos) { return GetColorAtPos(Pos); });
	
	SettingsViews[GET_MEMBER_NAME_CHECKED(FNoisePreviewSettings, Seed)]->SetOnPropertyValueChanged(
		FSimpleDelegate::CreateLambda([this]
		{
			OnSeedChanged.ExecuteIfBound(State.Settings.Seed);
			State.Update();
		})
	);
	
	SettingsViews[GET_MEMBER_NAME_CHECKED(FNoisePreviewSettings, Resolution)]->SetOnPropertyValueChanged(
		FSimpleDelegate::CreateLambda([this]
		{
			State.Update();
		})
	);
	
	OnSeedChanged.ExecuteIfBound(State.Settings.Seed);
	State.Update();
}

FColor SNoisePreviewWidget::GetColorAtPos(const FGridPosition& Pos) const
{
	if (OnGenerateColor.IsBound()) return OnGenerateColor.Execute(Pos);
	return FColor::Black;
}
