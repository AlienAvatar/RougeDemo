// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeBase.h"
#include "GameFramework/DamageType.h"
#include "FstDamageType.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UFstDamageType : public UDamageTypeBase
{
	GENERATED_BODY()

public:
	float SubstractToughness = 10.f;

	
};
