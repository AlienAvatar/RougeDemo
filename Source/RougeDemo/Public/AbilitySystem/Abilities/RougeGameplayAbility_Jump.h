// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility_Jump : public URougeGameplayAbility
{
	GENERATED_BODY()

public:

	URougeGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	void CharacterJumpStop();
};
