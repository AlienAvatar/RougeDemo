// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/RougeCombatSet.h"
#include "Net/UnrealNetwork.h"

URougeCombatSet::URougeCombatSet()
	: BaseDamage(0.0f)
	, Armor(0.0f)
{
}

void URougeCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeCombatSet, BaseDamage, OldValue);
}

void URougeCombatSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeCombatSet, Armor, OldValue);
}

void URougeCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URougeCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URougeCombatSet, Armor, COND_OwnerOnly, REPNOTIFY_Always);
}
