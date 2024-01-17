// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/RougeExperienceManagerComponent.h"

#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Assets/RougeAssetManager.h"
#include "Core/GameModes/RougeExperienceActionSet.h"
#include "Core/GameModes/RougeExperienceDefinition.h"
#include "Core/GameModes/RougeExperienceManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URougeExperienceManagerComponent::URougeExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void URougeExperienceManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// deactivate any features this experience loaded
	//@TODO: This should be handled FILO as well
	for (const FString& PluginURL : GameFeaturePluginURLs)
	{
		if (URougeExperienceManager::RequestToDeactivatePlugin(PluginURL))
		{
			UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
		}
	}

	//@TODO: Ensure proper handling of a partially-loaded state too
	if (LoadState == ERougeExperienceLoadState::Loaded)
	{
		LoadState = ERougeExperienceLoadState::Deactivating;

		FGameFeatureDeactivatingContext Context(FSimpleDelegate::CreateUObject(this, &ThisClass::OnActionDeactivationCompleted));
		auto DeactivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				if (Action)
				{
					Action->OnGameFeatureDeactivating(Context);
					Action->OnGameFeatureUnregistering();
				}
			}
		};
	}
}

void URougeExperienceManagerComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty, TSizedDefaultAllocator<32>>& LifetimeProperties) const
{
	Super::GetLifetimeReplicatedProps(LifetimeProperties);
	//DOREPLIFETIME(ThisClass, CurrentExperience);
}

void URougeExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnRougeExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

bool URougeExperienceManagerComponent::IsExperienceLoaded() const
{
	//当扩展组件加载完毕
	return (LoadState == ERougeExperienceLoadState::Loaded) && (CurrentExperience != nullptr);
}


// Called when the game starts
void URougeExperienceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URougeExperienceManagerComponent::OnRep_CurrentExperience()
{
}

void URougeExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience != nullptr);
	check(LoadState == ERougeExperienceLoadState::Unloaded);

	// UE_LOG(LogTemp, Log, TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s, %s)"),
	// *CurrentExperience->GetPrimaryAssetId().ToString(),
	// *GetClientServerContextString(this));
	
	//设置状态
	LoadState = ERougeExperienceLoadState::Loading;

	//获取URougeAssetManager
	URougeAssetManager& AssetManager = URougeAssetManager::Get();

	//Id List
	TSet<FPrimaryAssetId> BundleAssetList;
	TSet<FSoftObjectPath> RawAssetList;

	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
	for (const TObjectPtr<URougeExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
		}
	}

	//读取扩展相关的资源

	
	TArray<FName> BundlesToLoad;
	//BundlesToLoad.Add(FRougeBundles::Equipped);

	const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
	const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
	const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);

	if (bLoadClient)
	{
		//本地加载
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
	}
	if (bLoadServer)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}

	const TSharedPtr<FStreamableHandle> BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
	const TSharedPtr<FStreamableHandle> RawLoadHandle = AssetManager.LoadAssetList(RawAssetList.Array(), FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority, TEXT("StartExperienceLoad()"));

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
}

void URougeExperienceManagerComponent::OnExperienceLoadComplete()
{
	check(LoadState == ERougeExperienceLoadState::Loading);
	check(CurrentExperience != nullptr);

	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This=this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
	{
		
	};
}

void URougeExperienceManagerComponent::OnActionDeactivationCompleted()
{
	check(IsInGameThread());
	++NumObservedPausers;

	if (NumObservedPausers == NumExpectedPausers)
	{
		OnAllActionsDeactivated();
	}
}

void URougeExperienceManagerComponent::OnAllActionsDeactivated()
{
	//To Do
	LoadState = ERougeExperienceLoadState::Unloaded;
}

