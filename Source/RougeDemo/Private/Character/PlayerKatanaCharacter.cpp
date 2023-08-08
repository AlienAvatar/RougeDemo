// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerKatanaCharacter.h"

#include "Character/RougeDemoAnimInstance.h"
#include "Components/CombatComponent.h"


APlayerKatanaCharacter::APlayerKatanaCharacter()
{
	KatanaMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KatanaMeshComp"));
	KatanaMeshComp->SetupAttachment(GetMesh());
	
	/*const USkeletalMeshSocket* HandSocket2 = GetMesh()->GetSocketByName(FName("R_Weapon_KatanaBack_Socket"));
	const FTransform RightHandTransform = HandSocket2->GetSocketTransform(GetMesh());
	if(HandSocket2)
	{
		HandSocket2->AttachActor(CurrentWeapon,RougeDemoCharacter->GetMesh());
	}*/
}

void APlayerKatanaCharacter::TestAction()
{
	if(CombatComp == nullptr){ return;}

	float Duration = 0.2f;
	if(OverlayState == EOverlayState::EOS_Katana)
	{
		OverlayState = EOverlayState::EOS_Default;
		CombatComp->UnArmWeapon();
		
		//检查播放蒙太奇动画
		if(bCanPlayMontage)
		{
			bCanPlayMontage = false;

			//速度小于10，用root收刀，>10,用Inplace收刀
			if(Speed < 10.f)
			{
				if(UnArmAnimMontagesRoot)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(UnArmAnimMontagesRoot,1.f);
				}
			}else
			{
				if(UnArmAnimMontageInPlace)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(UnArmAnimMontageInPlace,1.5f);
					Duration /= 1.5f;
				}
			}

			GetWorld()->GetTimerManager().SetTimer(
				MontageFinishTimer,
				this,
				&APlayerKatanaCharacter::MontageFinishTimerCallBack,
				Duration
			);
		}
	}else
	{
		CombatComp->EquipWeapon();
		OverlayState = EOverlayState::EOS_Katana;

		if(bCanPlayMontage)
		{
			bCanPlayMontage = false;

			if(Speed < 10.f)
			{
				if(EquipAnimMontageRoot)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(EquipAnimMontageRoot,1.f);
				}
			}else
			{
				if(EquipAnimMontageInPlace)
				{
					Duration = RougeDemoAnimInstance->Montage_Play(EquipAnimMontageInPlace,1.5f);
					Duration /= 1.5f;
				}
			}
			

			GetWorld()->GetTimerManager().SetTimer(
				MontageFinishTimer,
				this,
				&APlayerKatanaCharacter::MontageFinishTimerCallBack,
				Duration
			);
		}
	}
}

void APlayerKatanaCharacter::MontageFinishTimerCallBack()
{
	bCanPlayMontage = true;
}

void APlayerKatanaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Test",IE_Pressed,this,&APlayerKatanaCharacter::TestAction);
}
