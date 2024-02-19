// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RougeDamageExecution.generated.h"

/**
 * 负责关键的最终伤害计算，继承UGameplayEffectExecutionCalculation，专门用于GE相关计算
 */
UCLASS()
class ROUGEDEMO_API URougeDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	URougeDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
