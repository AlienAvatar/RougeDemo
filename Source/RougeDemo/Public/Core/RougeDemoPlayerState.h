// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RougeDemoPlayerState.generated.h"

class ARougeDemoPlayerController;
class ARougeDemoCharacter;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeDemoPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ARougeDemoCharacter* RougeDemoCharacter;

	UPROPERTY()
	ARougeDemoPlayerController* RougeDemoPlayerController;
public:
	
	virtual void OnRep_Score() override;
	void AddToScore(float ScoreAmount);
};
