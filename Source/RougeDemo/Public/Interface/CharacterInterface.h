// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EPassiveAbilities.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

class USphereComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROUGEDEMO_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AdjustPassive(EPassiveAbilities Stat, float MultiplicationAmount) = 0;

	virtual void RestoreHealth(float Health) = 0;
};
