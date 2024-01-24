// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RougeGameplayAbility.generated.h"


/**
 * ERougeAbilityActivationPolicy
 *
 *	定义技能的激活
 */
UENUM(BlueprintType)
enum class ERougeAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};


/**
 * Rouge Gameplay技能 （ URougeGameplayAbility ）从UGameplayAbility类扩展而来，可提供额外的实用程序和与Rouge框架连接的接口
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnPawnAvatarSet();

	//当Ability system由Pawn初始化完成
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Ability Activation")
	ERougeAbilityActivationPolicy ActivationPolicy;
};
