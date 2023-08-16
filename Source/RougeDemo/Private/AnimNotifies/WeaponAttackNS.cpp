// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WeaponAttackNS.h"

#include "Character/PlayerKatanaCharacter.h"
#include "Components/CombatComponent.h"
#include "Weapon/MeleeWeapon.h"

void UWeaponAttackNS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	APlayerKatanaCharacter* KatanaCharacter = Cast<APlayerKatanaCharacter>(MeshComp->GetOwner());
	if(KatanaCharacter && KatanaCharacter->GetCombatComponent() && KatanaCharacter->GetCombatComponent()->GetCurrentWeapon())
	{
		AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(KatanaCharacter->GetCombatComponent()->GetCurrentWeapon());
		if(MeleeWeapon)
		{
			MeleeWeapon->BeginWeaponAttack(0.04f,2);
		}
	}
}

void UWeaponAttackNS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	APlayerKatanaCharacter* KatanaCharacter = Cast<APlayerKatanaCharacter>(MeshComp->GetOwner());
	if(KatanaCharacter && KatanaCharacter->GetCombatComponent() && KatanaCharacter->GetCombatComponent()->GetCurrentWeapon())
	{
		AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(KatanaCharacter->GetCombatComponent()->GetCurrentWeapon());
		if(MeleeWeapon)
		{
			MeleeWeapon->EndWeaponAttack();
		}
	}
}


