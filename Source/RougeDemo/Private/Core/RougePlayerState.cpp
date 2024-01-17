// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougePlayerState.h"

#include "Character/RougeCharacter.h"
#include "..\..\Public\Core\RougePlayerController.h"

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
