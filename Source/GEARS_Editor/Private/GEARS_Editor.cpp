#include "GEARS_Editor.h"

#include "Customization/BiomeTypeCustomization.h"
#include "Customization/GridSettingsCustomization.h"
#include "Customization/NoiseContextCustomization.h"
#include "Customization/SamplingContextCustomization.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/SamplingContext.h"
#include "Settings/GridSettings.h"

#define LOCTEXT_NAMESPACE "FGEARS_EditorModule"

void FGEARS_EditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FNoiseContext::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FNoiseContextCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSamplingContext::StaticStruct()->GetFName(),
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

void FGEARS_EditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		auto& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FNoiseContext::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSamplingContext::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UBiomeType::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UGridSettings::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGEARS_EditorModule, GEARS_Editor)