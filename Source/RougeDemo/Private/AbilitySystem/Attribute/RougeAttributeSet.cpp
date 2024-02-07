// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/RougeAttributeSet.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"

URougeAttributeSet::URougeAttributeSet()
{
}

UWorld* URougeAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

URougeAbilitySystemComponent* URougeAttributeSet::GetRougeAbilitySystemComponent() const
{
	return Cast<URougeAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}


