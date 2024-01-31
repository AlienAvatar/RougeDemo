// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/AbilityComponent.h"
#include "RougeAbilitySystemComponent.generated.h"

class URougeAbilityTagRelationshipMapping;
/**
 * Rouge技能系统组件（ URougeAbilitySystemComponent )）扩展了 技能系统组件（ UAbilitySystemComponent ）功能
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

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//如果设置了，这个表用于查找Tag的激活和取消关系
	UPROPERTY()
	TObjectPtr<URougeAbilityTagRelationshipMapping> TagRelationshipMapping;
};
