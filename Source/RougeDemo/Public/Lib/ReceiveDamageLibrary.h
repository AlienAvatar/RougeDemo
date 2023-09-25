// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReceiveDamageLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UReceiveDamageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static EMovementDirection CalculateDamageCauserDirection(AActor* DamagedActor, AActor* DamageCauser);
};
