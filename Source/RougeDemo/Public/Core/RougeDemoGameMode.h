// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/GameModeInterface.h"
#include "RougeDemoGameMode.generated.h"

class ARougeDemoPlayerController;
class ARougeDemoCharacter;
class AAIEnemyController;
class ABaseAI;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeDemoGameMode : public AGameMode, public IGameModeInterface
{
	GENERATED_BODY()

public:
	bool bGameIsPaused = false;
	
	virtual void PlayEliminated(ABaseAI* ElimmedCharacter, AAIEnemyController* VictimController,ARougeDemoPlayerController* AttackController);

	virtual void RequetRespwan(ACharacter* ElimmedCharacter,AController* ElimmedController);

	virtual void Pause(bool bPause, bool bOverride) override;
	
};
