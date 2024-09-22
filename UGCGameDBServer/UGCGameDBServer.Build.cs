// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UGCGameDBServer : ModuleRules
{
	public UGCGameDBServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.Add("Runtime/Launch/Public");

		PrivateIncludePaths.Add("Runtime/Launch/Private");      // For LaunchEngineLoop.cpp include

        PublicIncludePathModuleNames.Add("Launch");

        PrivateDependencyModuleNames.Add("Core");
		PrivateDependencyModuleNames.Add("Projects");
        //PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("ApplicationCore");
        PrivateDependencyModuleNames.Add("SimpleNetChannel");
        PrivateDependencyModuleNames.Add("SimpleMySQL");
        //PrivateDependencyModuleNames.Add("UGCGameCommon");

        PrivateDependencyModuleNames.Add("CoreUObject");
    }
}
