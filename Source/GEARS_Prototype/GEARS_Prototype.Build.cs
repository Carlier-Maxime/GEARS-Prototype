// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GEARS_Prototype : ModuleRules
{
	public GEARS_Prototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTasks",
			"DeveloperSettings",
			"AIModule",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore"
		});
	}
}
