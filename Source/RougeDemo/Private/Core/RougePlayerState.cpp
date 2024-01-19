// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougePlayerState.h"

#include "Character/RougeCharacter.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "Net/UnrealNetwork.h"

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

void ARougePlayerState::OnRep_PawnData()
{
}
