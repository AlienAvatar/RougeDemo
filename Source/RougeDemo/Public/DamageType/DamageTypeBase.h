// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageTypeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UDamageTypeBase : public UDamageType
{
	GENERATED_BODY()

public:
	float SubstractToughness = 0.f;
};
