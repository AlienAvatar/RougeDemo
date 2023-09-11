// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EState.h"
#include "Animation/AnimInstance.h"
#include "BaseAIAnimInstance.generated.h"

class ABaseAI;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UBaseAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	ABaseAI* BaseAI;
	
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float PitchOffset;

	virtual void RotateInPlaceCheck();

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	bool bRotateL;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	bool bRotateR;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	EState AnimState;

	float RotateMaxThreshold = 50.f;
	float RotateMinThreshold = -50.f;

	float MaxPlayRate = 3.0;
	float MinPlayRate = 1.15f;
	float AimYawRateMinRange = 90.f;
	float AimYawRateMaxRange = 270.f;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float RotateRate = 1.f;
	float AimYawRate = 0.f;

	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	float Direction;
};
