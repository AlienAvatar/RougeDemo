// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseExplosion.generated.h"

class UNiagaraSystem;

UCLASS()
class ROUGEDEMO_API ABaseExplosion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float Radius;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EDrawDebugTrace::Type> DebugTraceType = EDrawDebugTrace::Type::None;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 1.0f;

	void PlayVFX();

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* DamageFX;

	
public:

	void SetUp(float pDamage, float pRadius, UNiagaraSystem* pDamageFX);

	void DoWork();
};


