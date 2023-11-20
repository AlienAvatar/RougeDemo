// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Struct/GlobalOptionsStruct.h"
#include "GlobalOptionsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UGlobalOptionsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	FGlobalOptionsStruct GlobalOptions;
};
