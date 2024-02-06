// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RougeGameplayEffectContext.h"
#include "AbilitySystem/RougeAbilitySourceInterface.h"

FRougeGameplayEffectContext* FRougeGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FRougeGameplayEffectContext::StaticStruct()))
	{
		return (FRougeGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FRougeGameplayEffectContext::SetAbilitySource(const IRougeAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}
