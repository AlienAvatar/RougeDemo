// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RougeGlobalAbilitySystem.generated.h"

class URougeAbilitySystemComponent;


USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<URougeAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, URougeAbilitySystemComponent* ASC);
	void RemoveFromASC(URougeAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<URougeAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, URougeAbilitySystemComponent* ASC);
	void RemoveFromASC(URougeAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

/**
 * Rouge全局技能系统 （ URougeGlobalAbilitySystem ）提供了一种快速跟踪关卡中所有Rouge技能系统组件并与之交互的方法
 */
UCLASS()
class URougeGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	URougeGlobalAbilitySystem();

	
	UPROPERTY()
	TMap<TObjectPtr<URougeAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	
	void AddToASC(TSubclassOf<UGameplayAbility> Ability, URougeAbilitySystemComponent* ASC);
	void RemoveFromASC(URougeAbilitySystemComponent* ASC);
	
	/** 在RougeGlobalAbilitySystem中注册ASC，并应用任何有效的Global Effect/Ability*/
	void RegisterASC(URougeAbilitySystemComponent* ASC);
private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;
	
	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<URougeAbilitySystemComponent>> RegisteredASCs;
};
