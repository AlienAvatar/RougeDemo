// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MeleeWeapon.h"

#include "AI/BaseAI.h"
#include "Character/PlayerKatanaCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"


AMeleeWeapon::AMeleeWeapon()
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(GetWeaponMesh())
	{
		GetWeaponMesh()->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBeginOverlap);
	}
}

void AMeleeWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseAI* BaseAI = Cast<ABaseAI>(OtherActor);
	if(BaseAI)
	{
		FDamageEvent DamageEvent;
		APlayerKatanaCharacter* OwnerCharacter = Cast<APlayerKatanaCharacter>(GetOwner());
		if(OwnerCharacter)
		{
			AController* OwnerController = OwnerCharacter->Controller;
			if(OwnerController)
			{
				UE_LOG(LogTemp,Warning,TEXT("DamageCount[%f]"),DamageCount);
				UGameplayStatics::ApplyDamage(OtherActor,DamageCount,OwnerController,this,UDamageType::StaticClass());
			}
		}
	}
}


void AMeleeWeapon::BeginWeaponAttack(float NewAttackDelayTime, int NewAttackDelayCount)
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackDelayTime = NewAttackDelayTime;
	AttackDelayCount = NewAttackDelayCount;
}

void AMeleeWeapon::EndWeaponAttack()
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
