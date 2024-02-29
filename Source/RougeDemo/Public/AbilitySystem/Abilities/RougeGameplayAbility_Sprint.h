// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeGameplayAbility_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility_Sprint : public URougeGameplayAbility
{
	GENERATED_BODY()

public:

	URougeGameplayAbility_Sprint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	void CharacterSprintStart();

	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	void CharacterSprintStop();

private:
	UPROPERTY()
	ARougeCharacter* Player;

	float OriginWalkMaxSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxWalkSpeed = 900.f;
};
