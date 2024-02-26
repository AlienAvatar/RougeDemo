// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RougeAttributeSet.h"
#include "AttributeSet.h"
#include "Misc/AssertionMacros.h"
#include "RougeCombatSet.generated.h"

/**
 * 定义了施加伤害或治疗所必需的属性
 * 提供伤害和治疗支持。Gameplay效果将负责修改这些属性之一
 */
UCLASS(BlueprintType)
class ROUGEDEMO_API URougeCombatSet : public URougeAttributeSet
{
	GENERATED_BODY()

public:
	URougeCombatSet();
	
	ATTRIBUTE_ACCESSORS(URougeCombatSet, BaseDamage);

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Rouge|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;
protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);
private:



};
