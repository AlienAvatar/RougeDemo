// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RougeCharacter.h"
#include "RageRougeCharacter.generated.h"

UCLASS()
class ROUGEDEMO_API ARageRougeCharacter : public ARougeCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARageRougeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
