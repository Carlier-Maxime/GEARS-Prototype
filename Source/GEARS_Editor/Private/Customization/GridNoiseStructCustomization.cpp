#include "GridNoiseStructCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Subsystems/AssetEditorSubsystem.h"

TMap<FString, FNoisePreviewCache> FGridNoiseStructCustomization::PreviewCache;


TSharedRef<IPropertyTypeCustomization> FGridNoiseStructCustomization::MakeInstance()
{
	return MakeShareable(new FGridNoiseStructCustomization);
}

void FGridNoiseStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FGridNoiseStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	
	for (uint32 i=0; i<NumChildren; ++i)
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
	}
	
	const auto UniqueID = GetUniqueID(PropertyHandle);
	auto& Cache = CreateOrUpdateCache(PropertyHandle, UniqueID);
	
	constexpr auto EntryWidth = 128.f;
	
	IDetailGroup& PreviewGroup = ChildBuilder.AddGroup(FName("PreviewSection"), FText::FromString("Preview"));
	PreviewGroup.AddWidgetRow()
		.WholeRowContent()
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
				}).OnMouseButtonDown_Lambda([UniqueID](const FGeometry&, const FPointerEvent&)
				{
					if (const auto Subsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
					{
						Subsystem->OpenEditorForAsset(PreviewCache[UniqueID].Texture.Get());
						return FReply::Handled();
					}
					return FReply::Unhandled();
				})
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			.Padding(10, 5)
			[
				SNew(SGridPanel)

				// Preview Seed
				+ SGridPanel::Slot(0, 0).Padding(0, 5).VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Seed")).Font(IDetailLayoutBuilder::GetDetailFont())
				]
				+ SGridPanel::Slot(1, 0).Padding(5, 5)
				[
					SNew(SNumericEntryBox<int32>)
					.MinDesiredValueWidth(EntryWidth)
					.MinSliderValue(0)
					.MaxSliderValue(1000)
					.AllowSpin(true)
					.Value_Lambda([UniqueID](){ return PreviewCache[UniqueID].Seed; })
					.OnValueChanged_Lambda([UniqueID](int32 Val){ 
						PreviewCache[UniqueID].Seed = Val; 
						PreviewCache[UniqueID].Update(); 
					})
				]

				// Preview Resolution
				+ SGridPanel::Slot(0, 1).Padding(0, 5).VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Resolution")).Font(IDetailLayoutBuilder::GetDetailFont())
				]
				+ SGridPanel::Slot(1, 1).Padding(5, 5)
				[
					SNew(SNumericEntryBox<int32>)
					.MinDesiredValueWidth(EntryWidth)
					.MinSliderValue(32)
					.MaxSliderValue(512)
					.Delta(32)
					.AllowSpin(true)
					.Value_Lambda([UniqueID](){ return PreviewCache[UniqueID].Resolution; })
					.OnValueChanged_Lambda([UniqueID](int32 Val){ 
						PreviewCache[UniqueID].Resolution = FMath::Clamp(Val, 8, 1024);
						PreviewCache[UniqueID].Update(); 
					})
				]

				// Preview Thumbnail Resolution
				+ SGridPanel::Slot(0, 2).Padding(0, 5).VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Thumbnail Res")).Font(IDetailLayoutBuilder::GetDetailFont())
				]
				+ SGridPanel::Slot(1, 2).Padding(5, 5)
				[
					SNew(SNumericEntryBox<int32>)
					.MinDesiredValueWidth(EntryWidth)
					.MinSliderValue(64)
					.MaxSliderValue(256)
					.Delta(32)
					.AllowSpin(true)
					.Value_Lambda([this](){ return ThumbnailSize; })
					.OnValueChanged_Lambda([this](int32 Val){ 
						ThumbnailSize = Val;
					})
				]
			]
		];
	
	PreviewGroup.ToggleExpansion(true);
	
	PropertyHandle->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda([UniqueID]()
		{
			PreviewCache.FindOrAdd(UniqueID).Update();
		}));
}

FString FGridNoiseStructCustomization::GetUniqueID(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	TArray<UObject*> OuterObjects;
	PropertyHandle->GetOuterObjects(OuterObjects);
	
	if (OuterObjects.Num() > 0 && OuterObjects[0])
	{
		return OuterObjects[0]->GetPathName() + ":" + PropertyHandle->GeneratePathToProperty();
	}
	return PropertyHandle->GeneratePathToProperty();
}

FNoisePreviewCache& FGridNoiseStructCustomization::CreateOrUpdateCache(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	return CreateOrUpdateCache(PropertyHandle, GetUniqueID(PropertyHandle));
}

FNoisePreviewCache& FGridNoiseStructCustomization::CreateOrUpdateCache(const TSharedRef<IPropertyHandle>& PropertyHandle,
	const FString& UniqueID)
{
	void* StructPtr = nullptr;
	PropertyHandle->GetValueData(StructPtr);
	const UScriptStruct* StructType = CastField<FStructProperty>(PropertyHandle->GetProperty())->Struct;
	
	auto& Cache = PreviewCache.FindOrAdd(UniqueID);
	Cache.StructPtr = StructPtr;
	Cache.StructName = StructType->GetFName();
	Cache.Update();
	return Cache;
}

void FGridNoiseStructCustomization::ClearCache()
{
	PreviewCache.Empty();
}
