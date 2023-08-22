// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RougeDemoGameMode.generated.h"

class ARougeDemoPlayerController;
class ARougeDemoCharacter;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeDemoGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PlayEliminated(ARougeDemoCharacter* ElimmedCharacter, ARougeDemoPlayerController* VictimController,ARougeDemoPlayerController* AttackController);
};
