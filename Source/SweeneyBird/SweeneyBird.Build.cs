// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SweeneyBird : ModuleRules
{
	public SweeneyBird(ReadOnlyTargetRules TargetRules) : base(TargetRules)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D" });
	}
}
