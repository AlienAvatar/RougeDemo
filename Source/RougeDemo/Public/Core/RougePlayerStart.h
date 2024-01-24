// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "RougePlayerStart.generated.h"

/**
 * 游戏初始位置
 */
UCLASS()
class ROUGEDEMO_API ARougePlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ARougePlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
