// Fill out your copyright notice in the Description page of Project Settings.


#include "SNoisePreviewsWidget.h"

#include "SlateOptMacros.h"
#include "DetailLayoutBuilder.h"
#include "ISinglePropertyView.h"
#include "IStructureDataProvider.h"
#include "Grid/Types/WorldGridPos.h"
#include "Preview/NoisePreviewState.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNoisePreviewsWidget::Construct(const FArguments& InArgs)
{
	InitializeSettingsViews();
	OnSeedChanged = InArgs._OnSeedChanged;
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
		.Padding(5)
		.VAlign(VAlign_Center)
		[
			SAssignNew(PreviewsContainer, SHorizontalBox)
		]

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(10, 5)
		[
			Grid
		]
	];
}

void SNoisePreviewsWidget::AddNumericRow(
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

void SNoisePreviewsWidget::AddPreview(const TFunction<FColor(FWorldGridPos)>& GenColorFn)
{
	auto Index = Previews.Emplace();
	Previews[Index].OnGenerateColor.BindLambda(GenColorFn);
	PreviewsContainer->AddSlot()
        .AutoWidth()
        .Padding(5.f)
        .VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(&Previews[Index].Brush)
			.DesiredSizeOverride_Lambda([this]{
				return FVector2D{
					static_cast<double>(NoiseParams.ThumbnailSize),
					static_cast<double>(NoiseParams.ThumbnailSize)
				};
			}).OnMouseButtonDown_Lambda([this, Index](const FGeometry&, const FPointerEvent&)
			{
				const auto Subsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
				if (!Subsystem) return FReply::Unhandled();
				Subsystem->OpenEditorForAsset(Previews[Index].Texture.Get());
				return FReply::Handled();
			})
		];
	Previews[Index].Update(NoiseParams);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNoisePreviewsWidget::InitializeSettingsViews()
{
	SettingsStructOnScope = MakeShared<FStructOnScope>(FNoisePreviewContext::StaticStruct(), reinterpret_cast<uint8*>(&NoiseParams));
	TSharedRef<IStructureDataProvider> StructProvider = MakeShared<FStructOnScopeStructureDataProvider>(SettingsStructOnScope);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;
	
	const UScriptStruct* StructDefinition = FNoisePreviewContext::StaticStruct();
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

void SNoisePreviewsWidget::BindPropertyCallbacks(const TArray<TSharedRef<IPropertyHandle>>& PropertyHandles)
{
	const auto StateUpdateDelegate = FSimpleDelegate::CreateLambda([this]
	{
		for (auto& Preview : Previews) Preview.Update(NoiseParams);
	});
	
	for (auto& PropertyHandle : PropertyHandles)
	{
		if (!PropertyHandle->IsValidHandle()) continue;
		PropertyHandle->SetOnPropertyValueChanged(StateUpdateDelegate);
		PropertyHandle->SetOnChildPropertyValueChanged(StateUpdateDelegate);
	}
	
	SettingsViews[GET_MEMBER_NAME_CHECKED(FNoisePreviewContext, Seed)]->SetOnPropertyValueChanged(
		FSimpleDelegate::CreateLambda([this, StateUpdateDelegate]
		{
			OnSeedChanged.ExecuteIfBound(NoiseParams.Seed);
			StateUpdateDelegate.Execute();
		})
	);
	
	SettingsViews[GET_MEMBER_NAME_CHECKED(FNoisePreviewContext, Resolution)]->SetOnPropertyValueChanged(
		StateUpdateDelegate
	);
	
	SettingsViews[GET_MEMBER_NAME_CHECKED(FNoisePreviewContext, SamplingStep)]->SetOnPropertyValueChanged(
		StateUpdateDelegate
	);
	
	OnSeedChanged.ExecuteIfBound(NoiseParams.Seed);
}
