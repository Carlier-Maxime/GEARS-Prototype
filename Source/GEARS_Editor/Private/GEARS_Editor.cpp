#include "GEARS_Editor.h"

#include "GEARS_EditorUtils.h"
#include "Customization/BiomeTypeCustomization.h"
#include "Customization/GridSettingsCustomization.h"
#include "Customization/NoiseContextCustomization.h"
#include "Customization/SamplingContextCustomization.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/DistributionRule.h"

#define LOCTEXT_NAMESPACE "FGEARS_EditorModule"

void FGEARS_EditorModule::StartupModule()
{
	GeneratedAsset::OnRequestCreateAssetSync.BindStatic(&GEARSEditorUtils::HandleCreateAssetRequest);
	FCoreDelegates::OnPostEngineInit.AddLambda([]{RegisterCustomLayout();});
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
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSamplingContextCustomization::MakeInstance)
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

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGEARS_EditorModule, GEARS_Editor)