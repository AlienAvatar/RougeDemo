// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "RougeAttributeSet.generated.h"

class URougeAbilitySystemComponent;
class UWorld;

// 定义了一些初始化属性的函数
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Delegate used to broadcast attribute events.



/**
 * URougeAttributeSet
 * 在游戏机制中具有特定含义的一系列属性或数值。属性可以代表游戏资源，例如"生命值（Health）"、可以影响其他游戏规则的参考值，例如"基础攻击力（Base Attack Power）"，甚至可以代表无状态量，例如"施加的伤害（Applied Damage）"
 */
UCLASS()
class ROUGEDEMO_API URougeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	URougeAttributeSet();

	UWorld* GetWorld() const override;

	URougeAbilitySystemComponent* GetRougeAbilitySystemComponent() const;
};
