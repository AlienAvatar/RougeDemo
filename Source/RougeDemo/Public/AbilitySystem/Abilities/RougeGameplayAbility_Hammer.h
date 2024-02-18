// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeGameplayAbility_Hammer.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility_Hammer : public URougeGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//Hammer
	float HammerDamage = 0.5f;

	float HammerRadius = 300.0f;
	
	void GrantHammer(bool Cast);

	void PrepareHammer();

	void ExecuteHammer(TArray<FHitResult> Hits, float Damage, float Radius, APlayerController* Controller);

	FTimerHandle PrepareHammerTimerHandle;
	
	void PrepareHammerTimerHandleCallback();

protected:
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HammerFX;

};
