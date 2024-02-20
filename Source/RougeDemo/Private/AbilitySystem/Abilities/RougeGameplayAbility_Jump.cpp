// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/RougeGameplayAbility_Jump.h"

#include "Character/RougeCharacter.h"

URougeGameplayAbility_Jump::URougeGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool URougeGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

void URougeGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URougeGameplayAbility_Jump::CharacterJumpStart()
{
	if (ARougeCharacter* RougeCharacter = GetRougeCharacterFromActorInfo())
	{
		if (RougeCharacter->IsLocallyControlled() && !RougeCharacter->bPressedJump)
		{
			RougeCharacter->UnCrouch();
			RougeCharacter->Jump();
		}
	}
}

void URougeGameplayAbility_Jump::CharacterJumpStop()
{
	if (ARougeCharacter* RougeCharacter = GetRougeCharacterFromActorInfo())
	{
		if (RougeCharacter->IsLocallyControlled() && RougeCharacter->bPressedJump)
		{
			RougeCharacter->StopJumping();
		}
	}
}
