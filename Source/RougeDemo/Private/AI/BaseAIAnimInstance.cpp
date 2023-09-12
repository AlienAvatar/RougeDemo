// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIAnimInstance.h"

#include "AIController.h"
#include "KismetAnimationLibrary.h"
#include "AI/BaseAI.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

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

	//是否聚焦目标
	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(GetOwningActor());
	if(AIController)
	{
		bFocusTarget = IsValid(AIController->GetFocusActor());
	}
	
}

void UBaseAIAnimInstance::RotateInPlaceCheck()
{
	
}
