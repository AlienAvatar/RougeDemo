// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeGameplayCueManager.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "Engine/AssetManager.h"

URougeGameplayCueManager::URougeGameplayCueManager(const FObjectInitializer& ObjectInitializer)
{
}

URougeGameplayCueManager* URougeGameplayCueManager::Get()
{
	return Cast<URougeGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
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
