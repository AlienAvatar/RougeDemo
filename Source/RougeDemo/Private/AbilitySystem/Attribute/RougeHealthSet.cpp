// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/RougeHealthSet.h"

#include "Net/UnrealNetwork.h"

URougeHealthSet::URougeHealthSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
	, Mana(100.0f)
	, MaxMana(100.f)
	, Rage(0.0f)
	, MaxRage(100.0f)
{
}

void URougeHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URougeHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URougeHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void URougeHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeHealthSet, Health, OldValue);
}

void URougeHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeHealthSet, MaxHealth, OldValue);
}

void URougeHealthSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeHealthSet, Mana, OldValue);
}

void URougeHealthSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeHealthSet, MaxMana, OldValue);
}

void URougeHealthSet::OnRep_Rage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeHealthSet, Rage, OldValue);
}

void URougeHealthSet::OnRep_MaxRage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeHealthSet, MaxRage, OldValue);
}

bool URougeHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void URougeHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void URougeHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void URougeHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void URougeHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
