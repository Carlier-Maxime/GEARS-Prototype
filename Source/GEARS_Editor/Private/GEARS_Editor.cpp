#include "GEARS_Editor.h"

#include "Customization/GridNoiseDetailCustomization.h"
#include "Customization/NoiseContextCustomization.h"
#include "Customization/SamplingContextCustomization.h"
#include "Data/BiomeType.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Grid/Generator/Context/SamplingContext.h"

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
		FOnGetDetailCustomizationInstance::CreateStatic(&FGridNoiseDetailCustomization::MakeInstance)
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
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGEARS_EditorModule, GEARS_Editor)