#include "GEARS_Editor.h"

#include "Customization/GridNoiseStructCustomization.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/SamplingContext.h"

#define LOCTEXT_NAMESPACE "FGEARS_EditorModule"

void FGEARS_EditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FNoiseContext::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGridNoiseStructCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSamplingContext::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGridNoiseStructCustomization::MakeInstance)
	);

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FGEARS_EditorModule::ShutdownModule()
{
	FGridNoiseStructCustomization::ClearCache();
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		auto& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FNoiseContext::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSamplingContext::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGEARS_EditorModule, GEARS_Editor)