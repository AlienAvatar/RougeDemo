// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeDemoAnimInstance.h"

#include "Character/RougeDemoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URougeDemoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RougeDemoCharacter = Cast<ARougeDemoCharacter>(TryGetPawnOwner());
}

void URougeDemoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(RougeDemoCharacter == nullptr)
	{
		RougeDemoCharacter = Cast<ARougeDemoCharacter>(TryGetPawnOwner());
	}

	if(RougeDemoCharacter == nullptr)
	{
		return;
	}

	FVector Velocity = RougeDemoCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsAccelerating = RougeDemoCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	PitchOffset = RougeDemoCharacter->GetAO_Pitch();
	YawOffset = RougeDemoCharacter->GetAO_Yaw();
	//UE_LOG(LogTemp,Warning,TEXT("YawOffset[%f]"),YawOffset);
	//UE_LOG(LogTemp,Warning,TEXT("PitchOffset[%f]"),PitchOffset);
}
