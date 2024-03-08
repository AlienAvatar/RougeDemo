// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Assets/RougeAssetManagerStartupJob.h"
#include "Assets/RougeGameData.h"
#include "RougeAssetManager.generated.h"

struct FRougeBundles
{
	static const FName Equipped;
};

UCLASS(Config = Game)
class URougeAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	URougeAssetManager();

	// Sets up the ability system
	void InitializeAbilitySystem();
	void InitializeGameplayCueManager();
	
	const URougeGameData& GetGameData();

	// 返回URougeAssetManager的单例模式
	static URougeAssetManager& Get();

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);
protected:
	//UAssetManager Interface
	virtual void StartInitialLoading() override;

	//读取资源碎片百分比
	void UpdateInitialGameContentLoadPercent(float GameContentPercent);
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	// 线程安全的方式添加数据到内存中
	void AddLoadedAsset(const UObject* Asset);
	
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		// Does a blocking load if needed
		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}

	//Transit 属性是临时的，无法被保存或加载
	//全局的数据加载
	//在初始化的时候需要加载
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

	//当前值被存入相关的.ini文件中，创建后被加载
	UPROPERTY(Config)
	TSoftObjectPtr<URougeGameData> RougeGameDataPath;

	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);
private:
	TArray<FRougeAssetManagerStartupJob> StartupJobs;

	void DoAllStartupJobs();

	// AssetManager进行资产读取
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	//当更改数据的时候加锁
	FCriticalSection LoadedAssetsCritical;
};


template<typename AssetType>
TSubclassOf<AssetType> URougeAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}