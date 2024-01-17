// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "RougeExperienceManagerComponent.generated.h"

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
	
	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();
	
	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	//委托代理当扩展组件加载完毕
	FOnRougeExperienceLoaded OnExperienceLoaded;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const URougeExperienceDefinition> CurrentExperience;

};
