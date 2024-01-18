// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeGameState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Core/GameModes/RougeExperienceManagerComponent.h"

ARougeGameState::ARougeGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<URougeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent = CreateDefaultSubobject<URougeExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}

void ARougeGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARougeGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

void ARougeGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// UAbilitySystemComponent* ARougeGameState::GetAbilitySystemComponent() const
// {
// 	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
// }
