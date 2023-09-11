// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enum/EGait.h"
#include "EnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROUGEDEMO_API IEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	virtual bool SetMovementSpeed(EGait Gait);
};
