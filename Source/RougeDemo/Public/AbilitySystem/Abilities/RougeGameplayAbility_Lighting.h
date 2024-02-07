// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeGameplayAbility_Lighting.generated.h"

class UNiagaraSystem;
class ABaseExplosion;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility_Lighting : public URougeGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//Lightning
	void GrantLightning(bool Case);

	void PrepareLightningTimerHandleCallback();

	void PrepareLightning();

	FTimerHandle PrepareLightningTimerHandle;

	void ExecuteLightning(FVector TargetLocation, ACharacter* Instigator, float Damage, float Radius);

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* LightningSystem;
	
	FVector LastLightningLoc;

	float LightningDamage = 20.0f;
	
	float LightningRadius = 150.0f;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseExplosion> LightningClass;
};
