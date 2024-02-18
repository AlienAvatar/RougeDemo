// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RougeDamageExecution.generated.h"

/**
 * 
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
