// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RougeGameplayAbility_Death.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Character/RougeHealthComponent.h"
#include "RougeDemo/RougeGameplayTags.h"

URougeGameplayAbility_Death::URougeGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void URougeGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp,Warning,TEXT("URougeGameplayAbility_Death.ActivateAbility"))
	check(ActorInfo);

	URougeAbilitySystemComponent* RougeASC = CastChecked<URougeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	//带有Ability_Behavior_SurvivesDeath标签的则忽略
	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(FRougeGameplayTags::Get().Ability_Behavior_SurvivesDeath);
	
	// 取消所有技能并阻止其他的开始
	RougeASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
	SetCanBeCanceled(false);

	if (!CanChangeActivationGroup(ERougeAbilityActivationGroup::Exclusive_Blocking))
	{
		UE_LOG(LogTemp, Error, TEXT("URougeGameplayAbility_Death::ActivateAbility: Ability [%s] failed to change activation group to blocking."), *GetName());
	}
	
	if (bAutoStartDeath)
	{
		StartDeath();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URougeGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URougeGameplayAbility_Death::StartDeath()
{
	if (URougeHealthComponent* HealthComponent = URougeHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == ERougeDeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}

void URougeGameplayAbility_Death::FinishDeath()
{
	if (URougeHealthComponent* HealthComponent = URougeHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == ERougeDeathState::NotDead)
		{
			HealthComponent->FinishDeath();
		}
	}
}


