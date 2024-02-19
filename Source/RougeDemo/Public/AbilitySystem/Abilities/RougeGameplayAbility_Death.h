// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeGameplayAbility_Death.generated.h"

/**
 * 自动配置为触发死亡Gameplay事件。取消所有其他技能，并向Pawn的生命值组件发出信号以开始死亡过程（这反过来会触发其余的游戏通知和状态更改）
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility_Death : public URougeGameplayAbility
{
	GENERATED_BODY()
	
public:
	URougeGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual auto EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool bWasCancelled) -> void override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Death")
	bool bAutoStartDeath;

	// Starts the death sequence.
	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	void StartDeath();

	// Finishes the death sequence.
	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	void FinishDeath();
};
