// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "AI/BaseAI.h"

void UBaseAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseAI = Cast<ABaseAI>(TryGetPawnOwner());
}

void UBaseAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(BaseAI == nullptr)
	{
		BaseAI = Cast<ABaseAI>(TryGetPawnOwner());
	}

	if(BaseAI == nullptr)
	{
		return;
	}

	Velocity = BaseAI->GetVelocity();
	Speed = BaseAI->GetSpeed();
	
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity,GetOwningActor()->GetActorRotation());
}

void UBaseAIAnimInstance::RotateInPlaceCheck()
{
	
}
