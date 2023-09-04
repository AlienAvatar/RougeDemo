// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/SwingBeforeAttack.h"

#include "Character/PlayerKatanaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void USwingBeforeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	APlayerKatanaCharacter* Player = Cast<APlayerKatanaCharacter>(MeshComp->GetOwner());
	if(Player)
	{
		if(Player -> GetMoveF() != 0.f)
		{
			Player->bSwingBeforeAttack = true;
		}
		//会出现卡帧现象
		//Player->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}

void USwingBeforeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	APlayerKatanaCharacter* Player = Cast<APlayerKatanaCharacter>(MeshComp->GetOwner());
	if(Player)
	{
		Player->bSwingBeforeAttack = false;
		//Player->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}
