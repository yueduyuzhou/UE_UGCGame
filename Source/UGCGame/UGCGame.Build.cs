// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UGCGame : ModuleRules
{
	public UGCGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "SimpleThread",
            "SimpleNetChannel",
            "RuntimeTransformer",
            "UMG",
            "Slate",
            "SlateCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"});

        //PrivateDependencyModuleNames.AddRange(new string[] {
        //    "Sockets",
        //    "Networking"});

        //"UGCGameCommon",
    }
}
