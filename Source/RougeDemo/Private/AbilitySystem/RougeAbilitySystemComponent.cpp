// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeAbilitySystemComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "AbilitySystem/RougeGlobalAbilitySystem.h"
#include "Assets/RougeAssetManager.h"
#include "Character/RougeAnimInstance.h"
#include "Character/RougeCharacter.h"
#include "RougeDemo/RougeDemo.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");


// Sets default values for this component's properties
URougeAbilitySystemComponent::URougeAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void URougeAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	//AbilityActorInfo 缓存了有关能力需要经常访问的所有者参与者的数据
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if(bHasNewPawnAvatar)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			//获取基本玩法Ability类
			URougeGameplayAbility* RougeAbilityCDO = CastChecked<URougeGameplayAbility>(AbilitySpec.Ability);
			//返回执行时Ability的实例化方式。这限制了一种能力在执行过程中所能做的事情
			//检查Ability是否实例化或是否每次都执行
			if (RougeAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				//获取所有实例化的Ability
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilityInstance : Instances)
				{
					URougeGameplayAbility* RougeAbilityInstance = CastChecked<URougeGameplayAbility>(AbilityInstance);
					RougeAbilityInstance->OnPawnAvatarSet();
				}
			}else
			{
				RougeAbilityCDO->OnPawnAvatarSet();
			}
		}

		// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		// 当我们确实有一个Pawn时，向RougeGlobalAbilitySytem中注册，我们等待到直到下一次需要avatar时
		if (URougeGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<URougeGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}

		if (URougeAnimInstance* RougeAnimInst = Cast<URougeAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			RougeAnimInst->InitializeWithAbilitySystem(this);
		}

		TryActivateAbilitiesOnSpawn();
	}
}

void URougeAbilitySystemComponent::SetTagRelationshipMapping(URougeAbilityTagRelationshipMapping* NewMapping)
{
	TagRelationshipMapping = NewMapping;
}

void URougeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void URougeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void URougeAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void URougeAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const URougeGameplayAbility* RougeAbilityCDO = CastChecked<URougeGameplayAbility>(AbilitySpec->Ability);

				if (RougeAbilityCDO->GetActivationPolicy() == ERougeAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const URougeGameplayAbility* RougeAbilityCDO = CastChecked<URougeGameplayAbility>(AbilitySpec->Ability);

					if (RougeAbilityCDO->GetActivationPolicy() == ERougeAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

bool URougeAbilitySystemComponent::IsActivationGroupBlocked(ERougeAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case ERougeAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		// 独立技能不会被打断
		bBlocked = false;
		break;

	case ERougeAbilityActivationGroup::Exclusive_Replaceable:
	case ERougeAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.
		bBlocked = (ActivationGroupCounts[(uint8)ERougeAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

void URougeAbilitySystemComponent::AddAbilityToActivationGroup(ERougeAbilityActivationGroup Group,
	URougeGameplayAbility* RougeAbility)
{
	check(RougeAbility);
	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);

	ActivationGroupCounts[(uint8)Group]++;

	const bool bReplicateCancelAbility = false;
	
	
	switch (Group)
	{
	case ERougeAbilityActivationGroup::Independent:
		// Independent abilities do not cancel any other abilities.

		break;

	case ERougeAbilityActivationGroup::Exclusive_Replaceable:
	case ERougeAbilityActivationGroup::Exclusive_Blocking:
		CancelActivationGroupAbilities(ERougeAbilityActivationGroup::Exclusive_Replaceable, RougeAbility, bReplicateCancelAbility);
		break;

	default:
		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)ERougeAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)ERougeAbilityActivationGroup::Exclusive_Blocking];
	if (!ensure(ExclusiveCount <= 1))
	{
		UE_LOG(LogTemp, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
	}
}

void URougeAbilitySystemComponent::RemoveAbilityFromActivationGroup(ERougeAbilityActivationGroup Group,
	URougeGameplayAbility* RougeAbility)
{
	check(RougeAbility);
	check(ActivationGroupCounts[(uint8)Group] > 0);

	ActivationGroupCounts[(uint8)Group]--;
}

void URougeAbilitySystemComponent::CancelActivationGroupAbilities(ERougeAbilityActivationGroup Group,
	URougeGameplayAbility* IgnoreRougeAbility, bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this, Group, IgnoreRougeAbility](const URougeGameplayAbility* RougeAbility, FGameplayAbilitySpecHandle Handle)
	{
		return ((RougeAbility->GetActivationGroup() == Group) && (RougeAbility != IgnoreRougeAbility));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}


void URougeAbilitySystemComponent::AddAbilityArr(FGameplayTag Tag, FGameplayAbilitySpec GameplayAbilitySpec)
{
	AbilityArr.Add(Tag);
	//激活Ability
	OnGiveAbility(GameplayAbilitySpec);
}

void URougeAbilitySystemComponent::TestAbilityArr()
{
	for(auto Ability : AbilityArr)
	{
		FString AbilityStr = Ability.ToString();

		UE_LOG(LogTemp, Warning, TEXT("AbilityStr[%s]"),*AbilityStr);
	}
}

// Called when the game starts
void URougeAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URougeAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	//任何时候，如果你打算遍历ActivatableAbilities.Items，一定要在你的循环中添加ABILITYLIST_SCOPE_LOCK();，以锁定列表不发生变化
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const URougeGameplayAbility* RougeAbilityCDO = CastChecked<URougeGameplayAbility>(AbilitySpec.Ability);
		RougeAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
	}
}

void URougeAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void URougeAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void URougeAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	URougeGameplayAbility* RougeAbility = CastChecked<URougeGameplayAbility>(Ability);

	AddAbilityToActivationGroup(RougeAbility->GetActivationGroup(), RougeAbility);
}

void URougeAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			ClientNotifyAbilityFailed(Ability, FailureReason);
			return;
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void URougeAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
	bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	if (const URougeGameplayAbility* RougeAbility = Cast<const URougeGameplayAbility>(Ability))
	{
		//当Ability激活失败
		//RougeAbility->OnGameplayAbilityEnded(FailureReason);
	}	
}

void URougeAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags,
	UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags,
	bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, BlockTags,
	                                      bExecuteCancelTags, CancelTags);
}

void URougeAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags,
	UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);

	
}

void URougeAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability,
                                                       const FGameplayTagContainer& FailureReason)
{
	if (const URougeGameplayAbility* RougeAbility = Cast<const URougeGameplayAbility>(Ability))
	{
		//当Ability激活失败
		RougeAbility->OnAbilityFailedToActivate(FailureReason);
	}	
}

void URougeAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability,
	const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void URougeAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc,
                                                         bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		URougeGameplayAbility* RougeAbilityCDO = CastChecked<URougeGameplayAbility>(AbilitySpec.Ability);

		if (RougeAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			// Cancel all the spawned instances, not the CDO.
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			for (UGameplayAbility* AbilityInstance : Instances)
			{
				URougeGameplayAbility* RougeAbilityInstance = CastChecked<URougeGameplayAbility>(AbilityInstance);

				if (ShouldCancelFunc(RougeAbilityInstance, AbilitySpec.Handle))
				{
					if (RougeAbilityInstance->CanBeCanceled())
					{
						RougeAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), RougeAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *RougeAbilityInstance->GetName());
					}
				}
			}
		}
		else
		{
			// Cancel the non-instanced ability CDO.
			if (ShouldCancelFunc(RougeAbilityCDO, AbilitySpec.Handle))
			{
				// Non-instanced abilities can always be canceled.
				check(RougeAbilityCDO->CanBeCanceled());
				RougeAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo(), bReplicateCancelAbility);
			}
		}
	}
}

// Called every frame
void URougeAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URougeAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	//获取GameData中的数据
	const TSubclassOf<UGameplayEffect> DynamicTagGE = URougeAssetManager::GetSubclass(URougeGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to find DynamicTagGameplayEffect [%s]."), *URougeGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagGE, 1.0f, MakeEffectContext());
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	if (!Spec)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to make outgoing spec for [%s]."), *GetNameSafe(DynamicTagGE));
		return;
	}

	Spec->DynamicGrantedTags.AddTag(Tag);

	ApplyGameplayEffectSpecToSelf(*Spec);
}

void URougeAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	const TSubclassOf<UGameplayEffect> DynamicTagGE = URougeAssetManager::GetSubclass(URougeGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveDynamicTagGameplayEffect: Unable to find gameplay effect [%s]."), *URougeGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(Tag));
	Query.EffectDefinition = DynamicTagGE;

	RemoveActiveEffects(Query);
}


