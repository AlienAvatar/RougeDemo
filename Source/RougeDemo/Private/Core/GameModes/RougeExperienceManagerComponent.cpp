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
#include "RougeDemo/RougeDemo.h"
#include "Settings/RougeSettingsLocal.h"

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
	return (LoadState == ERougeExperienceLoadState::Loaded)  && (CurrentExperience != nullptr);
}

const URougeExperienceDefinition* URougeExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ERougeExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}

void URougeExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	//获取AssetManger实例化
	URougeAssetManager& AssetManager = URougeAssetManager::Get();
	//根据ID，获取Experience资源路径
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	//根据AssetPath,反射得到Class
	TSubclassOf<URougeExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);
	//获取对应的Experience定义配置
	const URougeExperienceDefinition* Experience = GetDefault<URougeExperienceDefinition>(AssetClass);

	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	//设置为当前的Experience
	CurrentExperience = Experience;
	//开始读取Experience
	StartExperienceLoad();
}

void URougeExperienceManagerComponent::SetCurrentExperience(FPrimaryAssetId ExperienceId)
{
}


// Called when the game starts
void URougeExperienceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URougeExperienceManagerComponent::OnRep_CurrentExperience()
{
	StartExperienceLoad();
}

void URougeExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience != nullptr);
	//检查当前Experience的读取状态
	check(LoadState == ERougeExperienceLoadState::Unloaded);

	//Log Output: LogTemp: EXPERIENCE: StartExperienceLoad(CurrentExperience = RougeExperienceDefinition:BP_Default_RougeExperienceDefinition, Server)
	UE_LOG(LogTemp, Log, TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s, %s)"),
	*CurrentExperience->GetPrimaryAssetId().ToString(),
	*GetClientServerContextString(this));
	
	//设置当前Experience的读取状态为loading
	LoadState = ERougeExperienceLoadState::Loading;

	//获取URougeAssetManager
	URougeAssetManager& AssetManager = URougeAssetManager::Get();

	//Id List
	// FPrimaryAssetId 包括 Primary Asset Type 和 Asset Name
	TSet<FPrimaryAssetId> BundleAssetList;
	TSet<FSoftObjectPath> RawAssetList;

	//对URougeExperienceDefinition中的配置数据进行加载
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
		//服务端加载
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}

	//AssetManager.ChangeBundleStateForPrimaryAssets方法，其内部会搜索属于AssetBundles的所有资源，然后执行异步加载操作。加载完成后进入LoadingGameFeatures状态
	const TSharedPtr<FStreamableHandle> BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
	//按顺序加载资源
	const TSharedPtr<FStreamableHandle> RawLoadHandle = AssetManager.LoadAssetList(RawAssetList.Array(), FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority, TEXT("StartExperienceLoad()"));

	//FStreamableManager提供异步加载功能
	TSharedPtr<FStreamableHandle> Handle = nullptr;
	if (BundleLoadHandle.IsValid() && RawLoadHandle.IsValid())
	{
		Handle = AssetManager.GetStreamableManager().CreateCombinedHandle({ BundleLoadHandle, RawLoadHandle });
	}
	else
	{
		Handle = BundleLoadHandle.IsValid() ? BundleLoadHandle : RawLoadHandle;
	}
	
	//加载完成
	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// Assets were already loaded, call the delegate now
		//调用资源加载完成委托
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
		{
			//执行委托
			OnAssetsLoadedDelegate.ExecuteIfBound();
		}));
	}

	// 这组资产被预加载，但我们不会基于它来阻止体验的开始
	TSet<FPrimaryAssetId> PreloadAssetList;
	//@TODO: Determine assets to preload (but not blocking-ly)
	if (PreloadAssetList.Num() > 0)
	{
		AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
	}
}

void URougeExperienceManagerComponent::OnExperienceLoadComplete()
{
	check(LoadState == ERougeExperienceLoadState::Loading);
	check(CurrentExperience != nullptr);

	GameFeaturePluginURLs.Reset();

	//加载插件
	auto CollectGameFeaturePluginURLs = [This=this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
	{
		for (const FString& PluginName : FeaturePluginList)
		{
			FString PluginURL;
			//GameFeatureSubSystem中查找PluginURL
			if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, /*out*/ PluginURL))
			{
				This->GameFeaturePluginURLs.AddUnique(PluginURL);
			}
			else
			{
				ensureMsgf(false, TEXT("OnExperienceLoadComplete failed to find plugin URL from PluginName %s for experience %s - fix data, ignoring for this run"), *PluginName, *Context->GetPrimaryAssetId().ToString());
			}
		}
	};

	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);
	for (const TObjectPtr<URougeExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			CollectGameFeaturePluginURLs(ActionSet, ActionSet->GameFeaturesToEnable);
		}
	}

	// Load and activate the features	
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading > 0)
	{
		LoadState = ERougeExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			URougeExperienceManager::NotifyOfPluginActivation(PluginURL);
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();
	}
}

void URougeExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginsLoading--;

	if (NumGameFeaturePluginsLoading == 0)
	{
		OnExperienceFullLoadCompleted();
	}
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

void URougeExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ERougeExperienceLoadState::Loaded);

	// Insert a random delay for testing (if configured)
	// 用一个随机等待时间进行测试
	if (LoadState != ERougeExperienceLoadState::LoadingChaosTestingDelay)
	{
		//const float DelaySecs = 2.0f;
		float RandomTime = FMath::Max(0.0f, 0.0f + FMath::FRand() * 0.0f);
		const float DelaySecs = RandomTime;
		if (DelaySecs > 0.0f)
		{
			FTimerHandle DummyHandle;

			LoadState = ERougeExperienceLoadState::LoadingChaosTestingDelay;
			//延迟
			GetWorld()->GetTimerManager().SetTimer(DummyHandle, this, &ThisClass::OnExperienceFullLoadCompleted, DelaySecs, /*bLooping=*/ false);

			return;
		}
	}

	LoadState = ERougeExperienceLoadState::ExecutingActions;

	// Execute the actions
	FGameFeatureActivatingContext Context;

	// Only apply to our specific world context if set
	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (ExistingWorldContext)
	{
		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
	{
		for (UGameFeatureAction* Action : ActionList)
		{
			if (Action != nullptr)
			{
				//@TODO: The fact that these don't take a world are potentially problematic in client-server PIE
				// The current behavior matches systems like gameplay tags where loading and registering apply to the entire process,
				// but actually applying the results to actors is restricted to a specific world
				Action->OnGameFeatureRegistering();
				Action->OnGameFeatureLoading();
				Action->OnGameFeatureActivating(Context);
			}
		}
	};

	ActivateListOfActions(CurrentExperience->Actions);
	for (const TObjectPtr<URougeExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = ERougeExperienceLoadState::Loaded;

	OnExperienceLoaded_HighPriority.Broadcast(CurrentExperience);
	OnExperienceLoaded_HighPriority.Clear();
	
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
	
	OnExperienceLoaded_LowPriority.Broadcast(CurrentExperience);
	OnExperienceLoaded_LowPriority.Clear();

	// Apply any necessary scalability settings
	#if !UE_SERVER
	URougeSettingsLocal::Get()->OnExperienceLoaded();
#endif
}

