// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RougeGameplayEffectContext.generated.h"

class IRougeAbilitySourceInterface;
class UObject;
/**
 * 
 */
USTRUCT()
struct FRougeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FRougeGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FRougeGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	//从句柄返回包装的FRougeGameplayEffectContext，如果它不存在或类型错误，则返回nullptr
	static ROUGEDEMO_API FRougeGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	//设置Ability Source
	void SetAbilitySource(const IRougeAbilitySourceInterface* InObject, float InSourceLevel);
	
protected:
	/** Ability Source object (should implement IRougeAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};
