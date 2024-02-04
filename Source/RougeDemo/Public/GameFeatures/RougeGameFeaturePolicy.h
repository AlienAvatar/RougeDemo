// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturesProjectPolicies.h"
#include "GameFeatureStateChangeObserver.h"
#include "RougeGameFeaturePolicy.generated.h"

/**
 * 自定义GameFeature加载策略
 * GFS GameFeatureSubsystem里会有一个Policy来决定是否加载某些GameFeature，方便做版本处理
 * GameFeature依赖AssetManager来加载资产
 */
UCLASS()
class ROUGEDEMO_API URougeGameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()

public:
	
	//~UGameFeaturesProjectPolicies interface
	//可以自定义GameFeature过滤器
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;
	//返回一个GameFeature要预先加载的资产列表，方便预载一些资产
	virtual TArray<FPrimaryAssetId> GetPreloadAssetListForGameFeature(const UGameFeatureData* GameFeatureToLoad, bool bIncludeLoadedAssets = false) const override;
	virtual bool IsPluginAllowed(const FString& PluginURL) const override;
	//
	virtual const TArray<FName> GetPreloadBundleStateForGameFeature() const override;
	virtual void GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const override;
	//~End of UGameFeaturesProjectPolicies interface

private:
	//监听每个GameFeature的状态变化
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

// checked
UCLASS()
class URougeGameFeature_HotfixManager : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureLoading(const UGameFeatureData* GameFeatureData, const FString& PluginURL) override;
};

// checked
//Action 
UCLASS()
class URougeGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	//当GameFeatureData加载的时候会调用这个
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
};