// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoPlayerState.h"

#include "Character/RougeDemoCharacter.h"
#include "Core/RougeDemoPlayerController.h"

void ARougeDemoPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	RougeDemoCharacter = RougeDemoCharacter == nullptr? Cast<ARougeDemoCharacter>(GetPawn()) : RougeDemoCharacter;
	if(RougeDemoCharacter)
	{
		RougeDemoPlayerController = RougeDemoPlayerController == nullptr ? Cast<ARougeDemoPlayerController>(RougeDemoCharacter->Controller) : RougeDemoPlayerController;
		if(RougeDemoPlayerController)
		{
			RougeDemoPlayerController->SetHUDScore(GetScore());
		}
	}
}

void ARougeDemoPlayerState::AddToScore(float ScoreAmount)
{
	float LocalScore = GetScore();
	LocalScore += ScoreAmount;
	SetScore(LocalScore);
	
	RougeDemoCharacter = RougeDemoCharacter == nullptr? Cast<ARougeDemoCharacter>(GetPawn()) : RougeDemoCharacter;
	if(RougeDemoCharacter)
	{
		RougeDemoPlayerController = RougeDemoPlayerController == nullptr ? Cast<ARougeDemoPlayerController>(RougeDemoCharacter->Controller) : RougeDemoPlayerController;
		if(RougeDemoPlayerController)
		{
			RougeDemoPlayerController->SetHUDScore(GetScore());
		}
	}
}
