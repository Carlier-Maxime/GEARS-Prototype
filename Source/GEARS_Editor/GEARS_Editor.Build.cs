using UnrealBuildTool;

public class GEARS_Editor : ModuleRules
{
    public GEARS_Editor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"GEARS_Prototype"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore",
			"PropertyEditor",
			"EditorStyle",
			"UnrealEd"
		});
    }
}