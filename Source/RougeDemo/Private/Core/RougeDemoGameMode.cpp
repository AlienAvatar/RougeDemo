// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougeGameMode.h"

#include "AI/AIEnemyController.h"
#include "AI/BaseAI.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "..\..\Public\Core\RougePlayerState.h"
#include "Character/RougeCharacter.h"
#include "Core/RougeGameSession.h"
#include "Core/RougeGameState.h"
#include "Core/RougeReplayPlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "HUD/RougeHUD.h"
#include "Kismet/GameplayStatics.h"

ARougeGameMode::ARougeGameMode()
{
	GameStateClass = ARougeGameState::StaticClass();
	GameSessionClass = ARougeGameSession::StaticClass();
	PlayerControllerClass = ARougePlayerController::StaticClass();
	ReplaySpectatorPlayerControllerClass = ARougeReplayPlayerController::StaticClass();
	PlayerStateClass = ARougePlayerState::StaticClass();
	DefaultPawnClass = ARougeCharacter::StaticClass();
	HUDClass = ARougeHUD::StaticClass();
}

void ARougeGameMode::PlayEliminated(ABaseAI* ElimmedCharacter,
                                    AAIEnemyController* VictimController, ARougePlayerController* AttackController)
{
	//计算Kills
	ARougePlayerController* RougeDemoPlayerController = Cast<ARougePlayerController>(AttackController);
	if(RougeDemoPlayerController)
	{
		ARougePlayerState* AttackerPlayerState = RougeDemoPlayerController ? Cast<ARougePlayerState>(RougeDemoPlayerController->PlayerState) : nullptr;
		if(AttackerPlayerState)
		{
			AttackerPlayerState->AddToScore(1.f);
		}
	}
}

void ARougeGameMode::RequetRespwan(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if(ElimmedCharacter)
	{
		//Controller Unproess
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	
	if(ElimmedController)
	{
		//查找PlayerStart，并重新生成Player
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(),Actors);

		int32 Selection = FMath::RandRange(0,Actors.Num()-1);
		RestartPlayerAtPlayerStart(ElimmedController,Actors[Selection]);
	}
}

void ARougeGameMode::Pause(bool bPause, bool bOverride)
{
	if(bPause)
	{
		//时间膨胀, 时间膨胀为0时，相当于暂停
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.0f);
		bGameIsPaused = true;
	}else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.0f);
		bGameIsPaused = false;
	}
	
}
