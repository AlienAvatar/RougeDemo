// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RougeGameplayEffectContext.generated.h"

class IRougeAbilitySourceInterface;
class UObject;
/**
 * 用于定义要发送到 Gameplay提示通知（Gameplay Cue Notifies） 的附加数据成员和函数
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
	//负责将上下文指针转换为专用类型
	static ROUGEDEMO_API FRougeGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	//设置Ability Source
	void SetAbilitySource(const IRougeAbilitySourceInterface* InObject, float InSourceLevel);

	//执行HitResults和其他无法进行内存复制的成员的深拷贝
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FRougeGameplayEffectContext* NewContext = new FRougeGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	//返回 FRougeGameplayEffectContext::StaticStruct()，以便向蓝图提供正确的反射数据
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FRougeGameplayEffectContext::StaticStruct();
	}

	//为定义的所有额外成员添加复制
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	/** Ability Source object (should implement IRougeAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};
