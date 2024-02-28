// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RougeAttributeSet.h"
#include "RougeHealthSet.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FRougeAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeHealthSet : public URougeAttributeSet
{
	GENERATED_BODY()

public:
	URougeHealthSet();

	ATTRIBUTE_ACCESSORS(URougeHealthSet, Damage);
	ATTRIBUTE_ACCESSORS(URougeHealthSet, Health);
	ATTRIBUTE_ACCESSORS(URougeHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(URougeHealthSet, Mana);
	ATTRIBUTE_ACCESSORS(URougeHealthSet, MaxMana);

	mutable FRougeAttributeEvent OnOutOfHealth;
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Rage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxRage(const FGameplayAttributeData& OldValue);
	// Attribute Interface Set Start 
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	// Attribute Interface Set End
	
private:
	// 当前生命值属性.  生命值将受到最大生命值属性的限制.  生命值对修改器是隐藏的，所以只有执行才能修改它.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Rouge|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// 最大生命值属性. gameplay effects 可以修改它.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Rouge|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Rouge|Mana", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Rouge|Mana", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Rage, Category = "Rouge|Rage", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Rage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxRage, Category = "Rouge|Rage", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxRage;
	
	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	UPROPERTY(BlueprintReadOnly, Category="Rouge|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
