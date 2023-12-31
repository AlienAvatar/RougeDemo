// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "UObject/Interface.h"
#include "ControllerManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControllerManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROUGEDEMO_API IControllerManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnLevelUp() = 0;

	UFUNCTION()
	virtual void UpdateLevelBar(float Percent, int32 Level) = 0;

	UFUNCTION()
	virtual void UpdateTime(FText Time) = 0;

	virtual void UpdateHudHotbar(TMap<EActiveAbilities, int32> ActiveMap, TMap<EPassiveAbilities, int32> PassiveMap) = 0;
	
};
