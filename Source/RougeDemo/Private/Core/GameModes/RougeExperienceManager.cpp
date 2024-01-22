// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/RougeExperienceManager.h"

bool URougeExperienceManager::RequestToDeactivatePlugin(const FString PluginURL)
{
	return false;
}

void URougeExperienceManager::NotifyOfPluginActivation(const FString PluginURL)
{
	if (GIsEditor)
	{
		URougeExperienceManager* ExperienceManagerSubsystem = GEngine->GetEngineSubsystem<URougeExperienceManager>();
		check(ExperienceManagerSubsystem);

		// Track the number of requesters who activate this plugin. Multiple load/activation requests are always allowed because concurrent requests are handled.
		int32& Count = ExperienceManagerSubsystem->GameFeaturePluginRequestCountMap.FindOrAdd(PluginURL);
		++Count;
	}
}
