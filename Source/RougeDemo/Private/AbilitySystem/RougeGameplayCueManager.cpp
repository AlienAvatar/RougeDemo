// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeGameplayCueManager.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "GameplayTagsManager.h"
#include "Engine/AssetManager.h"

enum class ERougeEditorLoadMode
{
	// Loads all cues upfront; longer loading speed in the editor but short PIE times and effects never fail to play
	LoadUpfront,

	// Outside of editor: Async loads as cue tag are registered
	// In editor: Async loads when cues are invoked
	//   Note: This can cause some 'why didn't I see the effect for X' issues in PIE and is good for iteration speed but otherwise bad for designers
	PreloadAsCuesAreReferenced_GameOnly,

	// Async loads as cue tag are registered
	PreloadAsCuesAreReferenced
};

namespace RougeGameplayCueManagerCvars
{
	//显示所有通过RougeGameplayCueManager加载并且当前在内存中的资源
	static FAutoConsoleCommand CVarDumpGameplayCues(
		TEXT("Rouge.DumpGameplayCues"),
		TEXT("Shows all assets that were loaded via RougeGameplayCueManager and are currently in memory."),
		FConsoleCommandWithArgsDelegate::CreateStatic(URougeGameplayCueManager::DumpGameplayCues));

	static ERougeEditorLoadMode LoadMode = ERougeEditorLoadMode::LoadUpfront;
}

URougeGameplayCueManager::URougeGameplayCueManager(const FObjectInitializer& ObjectInitializer)
{
}

URougeGameplayCueManager* URougeGameplayCueManager::Get()
{
	return Cast<URougeGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

void URougeGameplayCueManager::LoadAlwaysLoadedCues()
{
	if (ShouldDelayLoadGameplayCues())
	{
		UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
		
		TArray<FName> AdditionalAlwaysLoadedCueTags;
		for (const FName& CueTagName : AdditionalAlwaysLoadedCueTags)
		{
			FGameplayTag CueTag = TagManager.RequestGameplayTag(CueTagName, /*ErrorIfNotFound=*/ false);
			if (CueTag.IsValid())
			{
				ProcessTagToPreload(CueTag, nullptr);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("URougeGameplayCueManager::AdditionalAlwaysLoadedCueTags contains invalid tag %s"), *CueTagName.ToString());
			}
		}
	}
}

void URougeGameplayCueManager::OnCreated()
{
	Super::OnCreated();
}

bool URougeGameplayCueManager::ShouldAsyncLoadRuntimeObjectLibraries() const
{
	return Super::ShouldAsyncLoadRuntimeObjectLibraries();
}

bool URougeGameplayCueManager::ShouldSyncLoadMissingGameplayCues() const
{
	return Super::ShouldSyncLoadMissingGameplayCues();
}

bool URougeGameplayCueManager::ShouldAsyncLoadMissingGameplayCues() const
{
	return Super::ShouldAsyncLoadMissingGameplayCues();
}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));

void URougeGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}

	FAssetBundleData BundleData;
	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);

	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}

void URougeGameplayCueManager::DumpGameplayCues(const TArray<FString>& Args)
{
}

void URougeGameplayCueManager::ProcessTagToPreload(const FGameplayTag& Tag, UObject* OwningObject)
{
	switch (RougeGameplayCueManagerCvars::LoadMode)
	{
	case ERougeEditorLoadMode::LoadUpfront:
		return;
	case ERougeEditorLoadMode::PreloadAsCuesAreReferenced_GameOnly:
#if WITH_EDITOR
		if (GIsEditor)
		{
			return;
		}
#endif
		break;
	case ERougeEditorLoadMode::PreloadAsCuesAreReferenced:
		break;
	}
}

bool URougeGameplayCueManager::ShouldDelayLoadGameplayCues() const
{
	const bool bClientDelayLoadGameplayCues = true;
	return !IsRunningDedicatedServer() && bClientDelayLoadGameplayCues;
}
