// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RougeDemo : ModuleRules
{
	public RougeDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreOnline",
			"CoreUObject",
			"ApplicationCore",
			"Engine",
			"PhysicsCore",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"AIModule",
			"ModularGameplay",
			"ModularGameplayActors",
			"DataRegistry",
			"ReplicationGraph",
			"GameFeatures",
			"SignificanceManager",
			"Hotfix",
			"Niagara",
			"ControlFlows",
			"PropertyPath", 
			"CommonGame", 
			"GameSubtitles", 
			"GameSettings"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"InputCore",
			"Slate",
			"SlateCore",
			"RenderCore",
			"DeveloperSettings",
			"EnhancedInput",
			"NetCore",
			"RHI",
			"Projects",
			"Gauntlet",
			"UMG",
			"CommonUI",
			"CommonInput",
			"CommonUser",
			"AudioMixer",
			"NetworkReplayStreaming",
			"ClientPilot",
			"AudioModulation",
			"EngineSettings",
			"DTLSHandlerComponent", 
			"MoviePlayer", 
			"AnimGraphRuntime", 
			"GameSubtitles", 
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
