// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AntiVirusTactical : ModuleRules
{
	public AntiVirusTactical(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "UMG", "Slate", "SlateCore"});
    }
}
