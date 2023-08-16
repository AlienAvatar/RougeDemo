// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AMeleeWeapon();
	void BeginWeaponAttack(float NewAttackDelayTime, int NewAttackDelayCount);

	void EndWeaponAttack();
protected:
	virtual void BeginPlay() override;
	
	float AttackDelayTime;

	int AttackDelayCount;

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly,Category=Damage)
	float DamageCount = 10.f;
private:
};
