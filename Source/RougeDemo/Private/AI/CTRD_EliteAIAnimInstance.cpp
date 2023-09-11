// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CTRD_EliteAIAnimInstance.h"

#include "AI/CTRD_EliteAI.h"
#include "Kismet/KismetMathLibrary.h"


void UCTRD_EliteAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EliteAI = Cast<ACTRD_EliteAI>(TryGetPawnOwner());
}

void UCTRD_EliteAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(EliteAI == nullptr)
	{
		EliteAI = Cast<ACTRD_EliteAI>(TryGetPawnOwner());
	}

	if(EliteAI == nullptr)
	{
		return;
	}
	YawOffset = EliteAI->GetYawOffset();
	PitchOffset = EliteAI->GetPitchOffset();
	AimYawRate = EliteAI->GetAimYawRate();
	
	if(AnimState == EState::ES_Passive)
	{
		RotateInPlaceCheck();
	}else
	{
		bRotateL = false;
		bRotateR = false;
	}
}

void UCTRD_EliteAIAnimInstance::RotateInPlaceCheck()
{
	bRotateL = YawOffset <= RotateMinThreshold;
	bRotateR = YawOffset >= RotateMaxThreshold;
	if(bRotateL || bRotateR)
	{
		RotateRate = UKismetMathLibrary::MapRangeClamped(
			AimYawRate,
			AimYawRateMinRange,
			AimYawRateMaxRange,
			MinPlayRate,
			MaxPlayRate
		);
	}
}