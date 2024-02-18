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
 */
UCLASS(BlueprintType)
class  URougeCombatSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URougeCombatSet();
	
	ATTRIBUTE_ACCESSORS(URougeCombatSet, BaseDamage);
protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);
private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Rouge|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

};
