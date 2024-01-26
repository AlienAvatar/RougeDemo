// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougePlayerState.h"

#include "AbilitySystemComponent.h"
#include "Character/RougeCharacter.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "AbilitySystem/RougeAbilitySet.h"
#include "AbilitySystem/RougeAbilitySystemComponent.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Core/RougeGameMode.h"
#include "Core/GameModes/RougeExperienceManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "RougeDemo/RougeDemo.h"

DEFINE_LOG_CATEGORY(LogPlayerState);
const FName ARougePlayerState::NAME_RougeAbilityReady("RougeAbilitiesReady");

ARougePlayerState::ARougePlayerState(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<URougeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ARougePlayerState::Reset()
{
	Super::Reset();
}

void ARougePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void ARougePlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void ARougePlayerState::OnDeactivated()
{
	Super::OnDeactivated();
}

void ARougePlayerState::OnReactivated()
{
	Super::OnReactivated();
}

void ARougePlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARougePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	if (GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		URougeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<URougeExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnRougeExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ARougePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}

void ARougePlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	RougeDemoCharacter = RougeDemoCharacter == nullptr? Cast<ARougeCharacter>(GetPawn()) : RougeDemoCharacter;
	if(RougeDemoCharacter)
	{
		RougeDemoPlayerController = RougeDemoPlayerController == nullptr ? Cast<ARougePlayerController>(RougeDemoCharacter->Controller) : RougeDemoPlayerController;
		if(RougeDemoPlayerController)
		{
			RougeDemoPlayerController->SetHUDScore(GetScore());
		}
	}
}

void ARougePlayerState::AddToScore(float ScoreAmount)
{
	float LocalScore = GetScore();
	LocalScore += ScoreAmount;
	SetScore(LocalScore);
	
	RougeDemoCharacter = RougeDemoCharacter == nullptr? Cast<ARougeCharacter>(GetPawn()) : RougeDemoCharacter;
	if(RougeDemoCharacter)
	{
		RougeDemoPlayerController = RougeDemoPlayerController == nullptr ? Cast<ARougePlayerController>(RougeDemoCharacter->Controller) : RougeDemoPlayerController;
		if(RougeDemoPlayerController)
		{
			RougeDemoPlayerController->SetHUDScore(GetScore());
		}
	}
}

void ARougePlayerState::SetPawnData(const URougePawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		PLAYER_STATE_LOG(Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	//给定Class，PawnData（属性），类对象，如果属性或Class无效，则编译失败
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const URougeAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
	
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_RougeAbilityReady);
	
	ForceNetUpdate();
}

UAbilitySystemComponent* ARougePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FOnRougeTeamIndexChangedDelegate* ARougePlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ARougePlayerState::OnRep_PawnData()
{
}

void ARougePlayerState::OnExperienceLoaded(const URougeExperienceDefinition* CurrentExperience)
{
	if (ARougeGameMode* RougeGameMode = GetWorld()->GetAuthGameMode<ARougeGameMode>())
	{
		if (const URougePawnData* NewPawnData = RougeGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			PLAYER_STATE_LOG(Error, TEXT("ARougePlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}
