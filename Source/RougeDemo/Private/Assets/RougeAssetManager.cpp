// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/RougeAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "RougeDemo/RougeDemo.h"
#include "../RougeDemo.h"
#include "RougeDemo/RougeGameplayTags.h"

//////////////////////////////////////////////////////////////////////

#define STARTUP_JOB_WEIGHTED(JobFunc, JobWeight) StartupJobs.Add(FRougeAssetManagerStartupJob(#JobFunc, [this](const FRougeAssetManagerStartupJob& StartupJob, TSharedPtr<FStreamableHandle>& LoadHandle){JobFunc;}, JobWeight))
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

void URougeAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("URougeAssetManager::StartInitialLoading");
	Super::StartInitialLoading();

	STARTUP_JOB(InitializeAbilitySystem());

	DoAllStartupJobs();
}

void URougeAssetManager::UpdateInitialGameContentLoadPercent(float GameContentPercent)
{
	
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
