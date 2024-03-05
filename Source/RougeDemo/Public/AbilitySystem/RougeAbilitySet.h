// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RougeGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "RougeAbilitySet.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class URougeAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FRougeAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URougeGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * FRougeAbilitySet_GameplayEffect
 *
 *	数据用于Gameplay Effects
 *	
 */
USTRUCT(BlueprintType)
struct FRougeAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

// 数据属性
USTRUCT(BlueprintType)
struct FRougeAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

};

USTRUCT(BlueprintType)
struct FRougeAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	//Clear Ability, 删除所有技能
	void TakeFromAbilitySystem(URougeAbilitySystemComponent* RougeASC);

protected:

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 *  Rouge技能集 （ URougeAbilitySet ）是一种可以从内容浏览器创建的 数据资产（Data Asset） 类型
 *  它包含一系列Gameplay技能、Gameplay效果和属性集，当应用技能集时，这些技能集将赋予RougeCharacter
 */
UCLASS()
class ROUGEDEMO_API URougeAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(URougeAbilitySystemComponent* RougeASC, FRougeAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

	TArray<FRougeAbilitySet_GameplayAbility> GetGrantedGameplayAbilities() { return GrantedGameplayAbilities; }

	//void AddGrantedGameplayAbilities(URougeAbilitySystemComponent* RougeASC,FRougeAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject);
	
protected:
	// Gameplay abilities to grant when this ability set is granted.
	// 一系列要赋予的Rouge Gameplay技能，以及赋予该技能的默认关卡和与该技能相关联的可选输入标签。
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FRougeAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	// 一系列要赋予的Gameplay效果，以及赋予该效果的默认关卡
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FRougeAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// 一系列要赋予的Gameplay属性，以及赋予该效果的默认关卡
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FRougeAbilitySet_AttributeSet> GrantedAttributes;
};
