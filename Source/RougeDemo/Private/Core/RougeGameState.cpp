// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeGameState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Core/GameModes/RougeExperienceManagerComponent.h"
#include "Net/UnrealNetwork.h"

extern ENGINE_API float GAverageFPS;

ARougeGameState::ARougeGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<URougeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent = CreateDefaultSubobject<URougeExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}

void ARougeGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ARougeGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}


void ARougeGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARougeGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//初始化Actor的技能信息
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

void ARougeGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARougeGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//服务器端更新ServerFPS
	if (GetLocalRole() == ROLE_Authority)
	{
		ServerFPS = GAverageFPS;
	}
}

void ARougeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerFPS);
}

// UAbilitySystemComponent* ARougeGameState::GetAbilitySystemComponent() const
// {
// 	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
// }
