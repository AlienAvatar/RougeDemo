// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/RougeHealthSet.h"

#include "Net/UnrealNetwork.h"

URougeHealthSet::URougeHealthSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
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
	
}

void URougeHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
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
