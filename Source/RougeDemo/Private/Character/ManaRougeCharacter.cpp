// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ManaRougeCharacter.h"

#include "Character/RougeManaComponent.h"


// Sets default values
AManaRougeCharacter::AManaRougeCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ManaComponent = CreateDefaultSubobject<URougeManaComponent>(TEXT("ManaComponent"));
	
}

void AManaRougeCharacter::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
	
	URougeAbilitySystemComponent* RougeASC = GetRougeAbilitySystemComponent();
	check(RougeASC);

	//Init Mana Component
	ManaComponent->InitializeWithAbilitySystem(RougeASC);
}

