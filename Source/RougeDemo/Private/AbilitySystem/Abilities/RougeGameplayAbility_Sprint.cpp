// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RougeGameplayAbility_Sprint.h"

#include "Character/RougeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

URougeGameplayAbility_Sprint::URougeGameplayAbility_Sprint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool URougeGameplayAbility_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                      const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ARougeCharacter* RougeCharacter = Cast<ARougeCharacter>(ActorInfo->AvatarActor.Get());
	if (!RougeCharacter || !RougeCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return true;
}

void URougeGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Player = Cast<ARougeCharacter>(ActorInfo->AvatarActor.Get());
	if(Player)
	{
		OriginWalkMaxSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
		CharacterSprintStart();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URougeGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterSprintStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URougeGameplayAbility_Sprint::CharacterSprintStart()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void URougeGameplayAbility_Sprint::CharacterSprintStop()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = OriginWalkMaxSpeed;
}
