// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/RougeGameFeaturePolicy.h"

#include "GameFeatureAction.h"
#include "GameFeatureData.h"
#include "GameFeatures/GameFeatureAction_AddGameplayCuePath.h"
#include "GameplayCueSet.h"
#include "AbilitySystem/RougeGameplayCueManager.h"

void URougeGameFeaturePolicy::InitGameFeatureManager()
{
	Super::InitGameFeatureManager();
}

void URougeGameFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();
}

TArray<FPrimaryAssetId> URougeGameFeaturePolicy::GetPreloadAssetListForGameFeature(
	const UGameFeatureData* GameFeatureToLoad, bool bIncludeLoadedAssets) const
{
	return Super::GetPreloadAssetListForGameFeature(GameFeatureToLoad, bIncludeLoadedAssets);
}

bool URougeGameFeaturePolicy::IsPluginAllowed(const FString& PluginURL) const
{
	return Super::IsPluginAllowed(PluginURL);
}

const TArray<FName> URougeGameFeaturePolicy::GetPreloadBundleStateForGameFeature() const
{
	return Super::GetPreloadBundleStateForGameFeature();
}

void URougeGameFeaturePolicy::GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const
{
	Super::GetGameFeatureLoadingMode(bLoadClientData, bLoadServerData);
}

//////////////////////////////////////////////////////////////////////
/// URougeGameFeature_HotfixManager Start
//////////////////////////////////////////////////////////////////////
void URougeGameFeature_HotfixManager::OnGameFeatureLoading(const UGameFeatureData* GameFeatureData,
	const FString& PluginURL)
{
	// if (URougeHotfixManager* HotfixManager = Cast<URougeHotfixManager>(UOnlineHotfixManager::Get(nullptr)))
	// {
	// 	HotfixManager->RequestPatchAssetsFromIniFiles();
	// }
}
//////////////////////////////////////////////////////////////////////
/// URougeGameFeature_HotfixManager End
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// URougeGameFeature_AddGameplayCuePaths Start
//////////////////////////////////////////////////////////////////////
void URougeGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData,
                                                                     const FString& PluginName, const FString& PluginURL)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(URougeGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering);
	
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->GetDirectoryPathsToAdd();
			
			if (URougeGameplayCueManager* GCM = URougeGameplayCueManager::Get())
			{
				UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
				const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;

				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					GCM->AddGameplayCueNotifyPath(MutablePath, /** bShouldRescanCueAssets = */ false);	
				}
				
				// Rebuild the runtime library with these new paths
				if (!DirsToAdd.IsEmpty())
				{
					GCM->InitializeRuntimeObjectLibrary();	
				}

				const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
				if (PreInitializeNumCues != PostInitializeNumCues)
				{
					GCM->RefreshGameplayCuePrimaryAsset();
				}
			}
		}
	}
}

void URougeGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData,
	const FString& PluginName, const FString& PluginURL)
{
	IGameFeatureStateChangeObserver::OnGameFeatureUnregistering(GameFeatureData, PluginName, PluginURL);
}
//////////////////////////////////////////////////////////////////////
/// URougeGameFeature_AddGameplayCuePaths End
//////////////////////////////////////////////////////////////////////