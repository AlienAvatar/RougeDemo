// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Struct/AvailableCharacter.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	int32 GoldCount;

	FAvailableCharacter AvailableCharacter;
};
