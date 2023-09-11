// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ROUGEDEMO_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly,Category=Damage)
	float DamageCount = 10.f;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* AttackBox;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UBoxComponent* GetAttackBox() const { return AttackBox; }

	
};
