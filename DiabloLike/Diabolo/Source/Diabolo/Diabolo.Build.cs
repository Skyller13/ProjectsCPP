// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Diabolo : ModuleRules
{
	public Diabolo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule","UMG" , "Slate", "SlateCore", "GameplayTasks" });
    }
}
