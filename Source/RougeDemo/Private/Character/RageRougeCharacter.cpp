// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RageRougeCharacter.h"


// Sets default values
ARageRougeCharacter::ARageRougeCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARageRougeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARageRougeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARageRougeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

