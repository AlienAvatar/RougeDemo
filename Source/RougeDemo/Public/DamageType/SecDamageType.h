// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeBase.h"
#include "GameFramework/DamageType.h"
#include "SecDamageType.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API USecDamageType : public UDamageTypeBase
{
	GENERATED_BODY()

	float DestructibleImpulse = 30.f;
};
