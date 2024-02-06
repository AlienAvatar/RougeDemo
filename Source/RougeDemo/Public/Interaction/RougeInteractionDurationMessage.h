// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "RougeInteractionDurationMessage.generated.h"
/**
 * 
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_INTERACTION_DURATION_MESSAGE);

USTRUCT(BlueprintType)
struct FRougeInteractionDurationMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	float Duration = 0;
};
