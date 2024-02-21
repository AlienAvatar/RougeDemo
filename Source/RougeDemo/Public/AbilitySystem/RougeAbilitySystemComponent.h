// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/RougeGameplayAbility.h"
#include "RougeAbilitySystemComponent.generated.h"

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

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	//如果设置了，这个表用于查找Tag的激活和取消关系
	UPROPERTY()
	TObjectPtr<URougeAbilityTagRelationshipMapping> TagRelationshipMapping;
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
};
