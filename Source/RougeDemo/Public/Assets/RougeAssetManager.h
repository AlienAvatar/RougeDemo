// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Assets/RougeAssetManagerStartupJob.h"
#include "RougeAssetManager.generated.h"

UCLASS()
class URougeAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	URougeAssetManager();

	// Sets up the ability system
	void InitializeAbilitySystem();

protected:
	//UAssetManager Interface
	virtual void StartInitialLoading() override;

	void UpdateInitialGameContentLoadPercent(float GameContentPercent);
private:
	TArray<FRougeAssetManagerStartupJob> StartupJobs;

	void DoAllStartupJobs();

	
};
