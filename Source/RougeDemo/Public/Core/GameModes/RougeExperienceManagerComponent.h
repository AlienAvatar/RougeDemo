// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "RougeExperienceManagerComponent.generated.h"

namespace UE::GameFeatures
{
	struct FResult;
}

class URougeExperienceDefinition;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRougeExperienceLoaded, const URougeExperienceDefinition* /*Experience*/);

enum class ERougeExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

namespace RougeConsoleVariables
{
	static float ExperienceLoadRandomDelayMin = 0.0f;
	static FAutoConsoleVariableRef CVarExperienceLoadRandomDelayMin(
		TEXT("Rouge.chaos.ExperienceDelayLoad.MinSecs"),
		ExperienceLoadRandomDelayMin,
		TEXT("This value (in seconds) will be added as a delay of load completion of the experience (along with the random value Rouge.chaos.ExperienceDelayLoad.RandomSecs)"),
		ECVF_Default);

	static float ExperienceLoadRandomDelayRange = 0.0f;
	static FAutoConsoleVariableRef CVarExperienceLoadRandomDelayRange(
		TEXT("Rouge.chaos.ExperienceDelayLoad.RandomSecs"),
		ExperienceLoadRandomDelayRange,
		TEXT("A random amount of time between 0 and this value (in seconds) will be added as a delay of load completion of the experience (along with the fixed value Rouge.chaos.ExperienceDelayLoad.MinSecs)"),
		ECVF_Default);

	
}

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeExperienceManagerComponent(const FObjectInitializer& ObjectInitializer);
	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface
	void GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty,class TSizedDefaultAllocator<32> > &)const;
	
	void CallOrRegister_OnExperienceLoaded(FOnRougeExperienceLoaded::FDelegate&& Delegate);

	bool IsExperienceLoaded() const;

	// 如果加载完成，返回当前的CurrentExperience
	const URougeExperienceDefinition* GetCurrentExperienceChecked() const;
#if WITH_SERVER_CODE
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
#endif

	void SetCurrentExperience(FPrimaryAssetId ExperienceId);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TArray<FString> GameFeaturePluginURLs;

	ERougeExperienceLoadState LoadState = ERougeExperienceLoadState::Unloaded;

	UFUNCTION()
	void OnRep_CurrentExperience();
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	
	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

	int32 NumGameFeaturePluginsLoading = 0;
	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;



	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const URougeExperienceDefinition> CurrentExperience;

	//Experience加载完成
	void OnExperienceFullLoadCompleted();

	/**
	* Delegate called when the experience has finished loading just before others
	* 委托调用，在其它未加载时当Experience完成加载，高优先级
	* (e.g., subsystems that set up for regular gameplay)
	*/
	FOnRougeExperienceLoaded OnExperienceLoaded_HighPriority;

	//委托代理当扩展组件加载完毕
	FOnRougeExperienceLoaded OnExperienceLoaded;
	
	//委托代理当扩展组件加载完毕 低优先级
	FOnRougeExperienceLoaded OnExperienceLoaded_LowPriority;
};
