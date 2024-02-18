// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/RougeCombatSet.h"
#include "Net/UnrealNetwork.h"

URougeCombatSet::URougeCombatSet()
	: BaseDamage(0.0f)
{
}

void URougeCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URougeCombatSet, BaseDamage, OldValue);
}

void URougeCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URougeCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
}
