// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougePawnExtensionComponent.h"
#include "../RougeGameplayTags.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Character/RougePawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "UObject/UObjectBaseUtility.h"

const FName URougePawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

URougePawnExtensionComponent::URougePawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = nullptr;
}

void URougePawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("RougePawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(URougePawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one RougePawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	// 注册初始化状态
	RegisterInitStateFeature();
}

void URougePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// 绑定监听所有Feature，是否有改变
	//调用将OnActorInitStateChanged函数绑定到组件管理器上的适当委托
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	//如果初始状态可能改变为InitState_Spawned状态的话，通知状态Manager，默认初始化其余部分
	ensure(TryToChangeInitState(FRougeGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void URougePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//卸载AbilitySystem
	UninitializeAbilitySystem();
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void URougePawnExtensionComponent::SetPawnData(const URougePawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		//UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}
	
	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void URougePawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(
	FSimpleMulticastDelegate::FDelegate Delegate)
{
	//检查是否有任何函数绑定到给定的用户对象
	if(!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		// 将回调函数添加到多播委托中
		// 多播委托可以添加多个回调函数，当委托被触发时，所有注册的回调函数都会被调用
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if(AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void URougePawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}


// void URougePawnExtensionComponent::OnRep_PawnData()
// {
// }

void URougePawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

// Called every frame
void URougePawnExtensionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URougePawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

bool URougePawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void URougePawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (DesiredState == FRougeGameplayTags::Get().InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
}

void URougePawnExtensionComponent::CheckDefaultInitialization()
{
	// 检查其它进度是否初始化完成，因为在检查我们的进度之前，我们可能依赖的任何其他功能的进度
	// UGameFrameworkInitStateInterface的方法
	CheckDefaultInitializationForImplementers();

	const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
 	static const TArray<FGameplayTag> StateChain = {
		InitTags.InitState_Spawned,
		InitTags.InitState_DataAvailable,
		InitTags.InitState_DataInitialized,
		InitTags.InitState_GameplayReady
	};
	
	//这将尝试从生成(只在BeginPlay中设置)到数据初始化阶段
	ContinueInitStateChain(StateChain);
}

void URougePawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	//NAME_ActorFeatureName为PawnExtension
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		const FRougeGameplayTags& InitTags = FRougeGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void URougePawnExtensionComponent::InitializeAbilitySystem(URougeAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		//AbilityComponent未更改
		return;
	}

	if (AbilitySystemComponent)
	{
		//清除旧的AbilityComponent
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InASC->GetAvatarActor();

	UE_LOG(LogTemp, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(InASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));

	if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	{
		//Log存在的角色属于存在于服务端还是客户端
		UE_LOG(LogTemp, Log, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);

		// There is already a pawn acting as the ASC's avatar, so we need to kick it out
		// This can happen on clients if they're lagged: their new pawn is spawned + possessed before the dead one is removed
		//如果出现延迟就发生在客户端
		ensure(!ExistingAvatar->HasAuthority());

		if (URougePawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
		{
			//卸载AbilitySystem
			OtherExtensionComponent->UninitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	if (ensure(PawnData))
	{
		InASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
	}
}

void URougePawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		FGameplayTagContainer AbilityTypesToIgnore;
		AbilityTypesToIgnore.AddTag(FRougeGameplayTags::Get().Ability_Behavior_SurvivesDeath);

		AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void URougePawnExtensionComponent::HandleControllerChanged()
{
	if (AbilitySystemComponent && (AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>()))
	{
		ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
		if (AbilitySystemComponent->GetOwnerActor() == nullptr)
		{
			UninitializeAbilitySystem();
		}
		else
		{
			AbilitySystemComponent->RefreshAbilityActorInfo();
		}
	}

	CheckDefaultInitialization();
}


