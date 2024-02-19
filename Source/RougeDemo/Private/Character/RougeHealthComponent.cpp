// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RougeHealthComponent.h"

#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "RougeDemo/RougeGameplayTags.h"

URougeHealthComponent::URougeHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	AbilitySystemComponent = nullptr;
	DeathState = ERougeDeathState::NotDead;
}

void URougeHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URougeHealthComponent, DeathState);
}

// Called when the game starts
void URougeHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void URougeHealthComponent::OnRep_DeathState(ERougeDeathState OldDeathState)
{
}

void URougeHealthComponent::StartDeath()
{
	if (DeathState != ERougeDeathState::NotDead)
	{
		return;
	}

	DeathState = ERougeDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FRougeGameplayTags::Get().Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void URougeHealthComponent::FinishDeath()
{
	if (DeathState != ERougeDeathState::DeathStarted)
	{
		return;
	}

	DeathState = ERougeDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FRougeGameplayTags::Get().Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

// Called every frame
void URougeHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

