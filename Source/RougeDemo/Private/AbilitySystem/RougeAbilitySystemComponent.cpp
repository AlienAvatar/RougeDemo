// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeAbilitySystemComponent.h"

#include "AbilitySystem/RougeGameplayAbility.h"
#include "AbilitySystem/RougeGlobalAbilitySystem.h"
#include "Character/RougeAnimInstance.h"
#include "Character/RougeCharacter.h"


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
		// 	//  UnrealBuildTool failed with exit code 0x00000006
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


// Called every frame
void URougeAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


