// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "MeleeWeapon.generated.h"

class UParticleSystemComponent;
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

	UFUNCTION(BlueprintCallable)
	void ActivateWeaponAttack();

	UFUNCTION(BlueprintCallable)
	void DeactivateWeaponAttack();

	virtual void PlayTrailFX();

	void EndTrailFX();
protected:
	virtual void BeginPlay() override;
	
	float AttackDelayTime;

	int AttackDelayCount;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TrailFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* TrailComp;

	
private:
};
