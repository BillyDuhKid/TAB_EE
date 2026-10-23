// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TAB_EE : ModuleRules
{
	public TAB_EE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TAB_EE",
			"TAB_EE/Variant_Platforming",
			"TAB_EE/Variant_Platforming/Animation",
			"TAB_EE/Variant_Combat",
			"TAB_EE/Variant_Combat/AI",
			"TAB_EE/Variant_Combat/Animation",
			"TAB_EE/Variant_Combat/Gameplay",
			"TAB_EE/Variant_Combat/Interfaces",
			"TAB_EE/Variant_Combat/UI",
			"TAB_EE/Variant_SideScrolling",
			"TAB_EE/Variant_SideScrolling/AI",
			"TAB_EE/Variant_SideScrolling/Gameplay",
			"TAB_EE/Variant_SideScrolling/Interfaces",
			"TAB_EE/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
