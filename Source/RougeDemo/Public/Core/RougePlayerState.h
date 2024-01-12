// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RougePlayerState.generated.h"

class ARougePlayerController;
class ARougeCharacter;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougePlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ARougeCharacter* RougeDemoCharacter;

	UPROPERTY()
	ARougePlayerController* RougeDemoPlayerController;
public:
	
	virtual void OnRep_Score() override;
	void AddToScore(float ScoreAmount);
};
