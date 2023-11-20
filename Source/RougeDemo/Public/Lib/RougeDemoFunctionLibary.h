// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RougeDemoFunctionLibary.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeDemoFunctionLibary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void GetRougeDemoGameInstance(UObject* WorldContextObject);
};
