// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAIAnimInstance.h"
#include "CTRD_EliteAIAnimInstance.generated.h"

class ACTRD_EliteAI;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UCTRD_EliteAIAnimInstance : public UBaseAIAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY()
	ACTRD_EliteAI* EliteAI;

	virtual void RotateInPlaceCheck();

	
};
