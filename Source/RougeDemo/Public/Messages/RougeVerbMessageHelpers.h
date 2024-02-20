// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RougeVerbMessageHelpers.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Rouge")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);
};
