#include "GEARS_Editor.h"

#include "ContentBrowserModule.h"
#include "GEARS_EditorUtils.h"
#include "Customization/BiomeTypeCustomization.h"
#include "Customization/GridSettingsCustomization.h"
#include "Customization/NoiseContextCustomization.h"
#include "Customization/DistributionRuleCustomization.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/DistributionRule.h"
#include "ContentBrowserExtensions/ThumbnailToTexture.h"

#define LOCTEXT_NAMESPACE "FGEARS_EditorModule"

void FGEARS_EditorModule::StartupModule()
{
	GeneratedAsset::OnRequestCreateAssetSync.BindStatic(&GEARSEditorUtils::HandleCreateAssetRequest);
	FCoreDelegates::OnPostEngineInit.AddLambda([this]{PostEngineInit();});
}

void FGEARS_EditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		auto& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FNoiseContext::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FDistributionRule::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UBiomeType::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UGridSettings::StaticClass()->GetFName());
	}
	
	if (FModuleManager::Get().IsModuleLoaded("ContentBrowser"))
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>("ContentBrowser");
		TArray<FContentBrowserMenuExtender_SelectedAssets>& Extenders = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		FThumbnailContentBrowserExtensions_Impl::UnregisterExtender(Extenders);
	}
}

void FGEARS_EditorModule::RegisterCustomLayout()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FNoiseContext::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FNoiseContextCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FDistributionRule::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDistributionRuleCustomization::MakeInstance)
	);
	
	PropertyModule.RegisterCustomClassLayout(
		UBiomeType::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FBiomeTypeCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomClassLayout(
		UGridSettings::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FGridSettingsCustomization::MakeInstance)
	);

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FGEARS_EditorModule::PostEngineInit()
{
	RegisterCustomLayout();

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FContentBrowserMenuExtender_SelectedAssets>& Extenders = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	FThumbnailContentBrowserExtensions_Impl::RegisterExtender(Extenders);
	FThumbnailContentBrowserExtensions_Impl::AutoGenerateThumbnails();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGEARS_EditorModule, GEARS_Editor)