// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RougeGameplayAbility.generated.h"

class IRougeAbilitySourceInterface;
struct FRougeGameplayEffectContext;
class ARougeCharacter;

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
 * ERougeAbilityActivationGroup
 *
 *	Defines how an ability activates in relation to other abilities.
 *	定义一个Ability和其它Ability之间的关系
 */
UENUM(BlueprintType)
enum class ERougeAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	//独立运行，不会被打断
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.
	//会被其它技能替代，如正在使用某个Ability，被其它Ability代替
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.
	//会被其它技能阻塞，无法使用某个Ability
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
};

/**
 * Rouge Gameplay技能 （ URougeGameplayAbility ）从UGameplayAbility类扩展而来，可提供额外的实用程序和与Rouge框架连接的接口
 * UGameplayAbility 一种游戏操作，可以赋予支持GAS的Actor并由其执行，附带用于确定其需求、开销和其他行为的信息。 示例包括基本的近战攻击到独立的游戏菜单流，再到由另一个游戏操作引起的触发行为
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnPawnAvatarSet();

	//当Ability system由Pawn初始化完成
	/*UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();*/

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	ERougeAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Rouge|Ability")
	ARougeCharacter* GetRougeCharacterFromActorInfo() const;
protected:
	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	// virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
	// //~End of UGameplayAbility interface

	//当该Ability被AbilitySystemComponent授予时，添加Ability
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

	//当该Ability被AbilitySystemComponent授予时，删除Ability
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();
	
	//定义Ability该如何激活
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Ability Activation")
	ERougeAbilityActivationPolicy ActivationPolicy;

	//定义激活的Ability和其它激活的Ability之间的关系
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Ability Activation")
	ERougeAbilityActivationGroup ActivationGroup;

	virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const IRougeAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;
};
