// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeAbilitySystemComponent.generated.h"

class URougeAttributeSet;
class URougeAbilityTagRelationshipMapping;

ROUGEDEMO_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

/**
 * Rouge技能系统组件（ URougeAbilitySystemComponent )）扩展了 技能系统组件（ UAbilitySystemComponent ）功能
 * 负责和GA、GE、AS打交道
 * Gameplay Ability(GA)标识了游戏中一个对象（Actor）可以做的行为或技能
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUGEDEMO_API URougeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URougeAbilitySystemComponent();

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	// 设置Tag的相关映射
	void SetTagRelationshipMapping(URougeAbilityTagRelationshipMapping* NewMapping);

	//Ability Input输入
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ClearAbilityInput();

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	//是否会被激活的组别阻塞
	bool IsActivationGroupBlocked(ERougeAbilityActivationGroup Group) const;
	UFUNCTION(BlueprintCallable)
	void AddAbilityToActivationGroup(ERougeAbilityActivationGroup Group, URougeGameplayAbility* RougeAbility);
	void RemoveAbilityFromActivationGroup(ERougeAbilityActivationGroup Group, URougeGameplayAbility* RougeAbility);
	void CancelActivationGroupAbilities(ERougeAbilityActivationGroup Group, URougeGameplayAbility* IgnoreRougeAbility, bool bReplicateCancelAbility);

	typedef TFunctionRef<bool(const URougeGameplayAbility* RougeAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	//如果设置了，这个表用于查找Tag的激活和取消关系
	UPROPERTY()
	TObjectPtr<URougeAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Uses a gameplay effect to add the specified dynamic granted tag.
	UFUNCTION(BlueprintCallable)
	void AddDynamicTagGameplayEffect(const FGameplayTag& Tag);

	// Removes all active instances of the gameplay effect that was used to add the specified dynamic granted tag.
	UFUNCTION(BlueprintCallable)
	void RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag);

	TArray<FGameplayTag> AbilityArr;

	void AddAbilityArr(FGameplayTag Tag, FGameplayAbilitySpec GameplayAbilitySpec);
	
	UFUNCTION(BlueprintCallable)
	void TestAbilityArr();
	//TArray<FGameplayTag, UGameplayAbility> AbilityMap;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void TryActivateAbilitiesOnSpawn();
	
	// 处理当前帧输入按下时的Ability
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// 处理当前帧输入放开时的Ability
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// 处理Ability保持输入
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// Number of abilities running in each activation group.
	//在Activation Group中正在运行Ability的个数
	int32 ActivationGroupCounts[(uint8)ERougeAbilityActivationGroup::MAX];

	//复制事件替代，让WaitInputPress可以工作
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	//复制事件替代，让WaitInputReleased可以工作
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;

	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	/** 通知客户端Ability激活失败 */
	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);
};
