// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/RougeAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "RougeDemo/RougeDemo.h"
#include "../RougeDemo.h"
#include "AbilitySystem/RougeGameplayCueManager.h"

#include "RougeDemo/RougeGameplayTags.h"

//////////////////////////////////////////////////////////////////////
// STARTUP_JOB_WEIGHTED宏接受两个参数：JobFunc和JobWeight， 它使用这些参数来创建一个FRougeAssetManagerStartupJob对象
#define STARTUP_JOB_WEIGHTED(JobFunc, JobWeight) StartupJobs.Add(FRougeAssetManagerStartupJob(#JobFunc, [this](const FRougeAssetManagerStartupJob& StartupJob, TSharedPtr<FStreamableHandle>& LoadHandle){JobFunc;}, JobWeight))
//STARTUP_JOB宏是STARTUP_JOB_WEIGHTED宏的简化版本，只接受一个参数JobFunc。它将任务权重设置为默认值1.0，并调用STARTUP_JOB_WEIGHTED宏来添加任务
#define STARTUP_JOB(JobFunc) STARTUP_JOB_WEIGHTED(JobFunc, 1.f)

//////////////////////////////////////////////////////////////////////
///
URougeAssetManager::URougeAssetManager()
{
}

void URougeAssetManager::InitializeAbilitySystem()
{
	SCOPED_BOOT_TIMING("URougeAssetManager::InitializeAbilitySystem");
	FRougeGameplayTags::InitializeNativeTags();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

void URougeAssetManager::InitializeGameplayCueManager()
{
	SCOPED_BOOT_TIMING("URougeAssetManager::InitializeGameplayCueManager");

	URougeGameplayCueManager* GCM = URougeGameplayCueManager::Get();
	check(GCM);
	GCM->LoadAlwaysLoadedCues();
}

void URougeAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("URougeAssetManager::StartInitialLoading");
	Super::StartInitialLoading();

	STARTUP_JOB(InitializeAbilitySystem());
	STARTUP_JOB(InitializeGameplayCueManager());

	DoAllStartupJobs();
}

void URougeAssetManager::UpdateInitialGameContentLoadPercent(float GameContentPercent)
{
	
}

const URougeGameData& URougeAssetManager::GetGameData()
{
	//RougeGameDataPath 从DefaultEngine.ini
	return GetOrLoadTypedGameData<URougeGameData>(RougeGameDataPath);
}

URougeAssetManager& URougeAssetManager::Get()
{
	check(GEngine)

	//Asset Manager 再Project Settings中配置
	if (URougeAssetManager* Singleton = Cast<URougeAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to RougeAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<URougeAssetManager>();
}

UPrimaryDataAsset* URougeAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass,
	const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType)
{
	UPrimaryDataAsset* Asset = nullptr;
	//要保证在GameEngine.ini中的配置正确
	if (!DataClassPath.IsNull())
	{
		//这可以在编辑器中递归调用，因为它是按需从PostLoad调用的，所以可以在编辑器中递归调用，因为它是按需从PostLoad调用的，所以强制同步加载主资产，并在这种情况下异步加载其余的主资产，在这种情况下异步加载其余的
		if (GIsEditor)
		{
			Asset = DataClassPath.LoadSynchronous();
			LoadPrimaryAssetsWithType(PrimaryAssetType);
		}else
		{
			TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
			if (Handle.IsValid())
			{
				Handle->WaitUntilComplete(0.0f, false);

				// This should always work
				Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
			}
		}
	}
	
	if (Asset)
	{
		GameDataMap.Add(DataClass, Asset);
	}
	else
	{
		// It is not acceptable to fail to load any GameData asset. It will result in soft failures that are hard to diagnose.
		UE_LOG(LogTemp, Fatal, TEXT("Failed to load GameData asset at %s. Type %s. This is not recoverable and likely means you do not have the correct data to run %s."), *DataClassPath.ToString(), *PrimaryAssetType.ToString(), FApp::GetProjectName());
	}

	return Asset;
}

void URougeAssetManager::DoAllStartupJobs()
{
	SCOPED_BOOT_TIMING("URougeAssetManager::DoAllStartupJobs");

	const double AllStartupJobsStartTime = FPlatformTime::Seconds();

	if (IsRunningDedicatedServer())
	{
		// No need for periodic progress updates, just run the jobs
		for (const FRougeAssetManagerStartupJob& StartupJob : StartupJobs)
		{
			StartupJob.DoJob();
		}
	}else
	{
		if (StartupJobs.Num() > 0)
		{
			float TotalJobValue = 0.0f;
			for (const FRougeAssetManagerStartupJob& StartupJob : StartupJobs)
			{
				TotalJobValue += StartupJob.JobWeight;
			}

			float AccumulatedJobValue = 0.0f;
			for (FRougeAssetManagerStartupJob& StartupJob : StartupJobs)
			{
				const float JobValue = StartupJob.JobWeight;
				StartupJob.SubstepProgressDelegate.BindLambda([This = this, AccumulatedJobValue, JobValue, TotalJobValue](float NewProgress)
					{
						const float SubstepAdjustment = FMath::Clamp(NewProgress, 0.0f, 1.0f) * JobValue;
						const float OverallPercentWithSubstep = (AccumulatedJobValue + SubstepAdjustment) / TotalJobValue;

						This->UpdateInitialGameContentLoadPercent(OverallPercentWithSubstep);
					});

				StartupJob.DoJob();

				StartupJob.SubstepProgressDelegate.Unbind();

				AccumulatedJobValue += JobValue;

				UpdateInitialGameContentLoadPercent(AccumulatedJobValue / TotalJobValue);
			}
		}
		else
		{
			UpdateInitialGameContentLoadPercent(1.0f);
		}
	}

	StartupJobs.Empty();

	UE_LOG(LogTemp, Warning, TEXT("All startup jobs took %.2f seconds to complete"), FPlatformTime::Seconds() - AllStartupJobsStartTime);
}
