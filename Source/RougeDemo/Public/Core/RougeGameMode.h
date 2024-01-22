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
class URougePawnData;
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

	UFUNCTION(BlueprintCallable, Category = "Rouge|Pawn")
	const URougePawnData* GetPawnDataForController(const AController* InController) const;
	
	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	//~End of AGameModeBase interface
protected:
	void HandleMatchAssignmentIfNotExpectingOne();
	
	void OnExperienceLoaded(const URougeExperienceDefinition* CurrentExperience);

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);
};
