// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MeleeWeapon.h"

#include "AI/BaseAI.h"
#include "Engine/DamageEvents.h"

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
		TakeDamage(DamageCount,DamageEvent,GetInstigatorController(),GetOwner());
	}
}


void AMeleeWeapon::BeginWeaponAttack(float NewAttackDelayTime, int NewAttackDelayCount)
{
	//打开Attack通道
	//GetWeaponMesh()->SetCollisionResponseToChannel(ECC_EngineTraceChannel2,ECR_Overlap);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackDelayTime = NewAttackDelayTime;
	AttackDelayCount = NewAttackDelayCount;
}

void AMeleeWeapon::EndWeaponAttack()
{
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
