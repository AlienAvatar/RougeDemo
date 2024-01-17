// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "Interface/GameModeInterface.h"
#include "RougeGameMode.generated.h"

class ARougePlayerController;
class ARougeCharacter;
class AAIEnemyController;
class ABaseAI;
class URougeExperienceDefinition;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeGameMode : public AModularGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()

public:
	ARougeGameMode();
	
	bool bGameIsPaused = false;
	
	virtual void PlayEliminated(ABaseAI* ElimmedCharacter, AAIEnemyController* VictimController,ARougePlayerController* AttackController);

	virtual void RequetRespwan(ACharacter* ElimmedCharacter,AController* ElimmedController);

	virtual void Pause(bool bPause, bool bOverride) override;

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	//~End of AGameModeBase interface
protected:
	void HandleMatchAssignmentIfNotExpectingOne();
	
	void OnExperienceLoaded(const URougeExperienceDefinition* CurrentExperience);
};
