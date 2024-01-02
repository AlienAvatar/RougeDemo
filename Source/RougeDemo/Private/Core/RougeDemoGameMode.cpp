// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoGameMode.h"

#include "AI/AIEnemyController.h"
#include "AI/BaseAI.h"
#include "Core/RougeDemoPlayerController.h"
#include "Core/RougeDemoPlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ARougeDemoGameMode::PlayEliminated(ABaseAI* ElimmedCharacter,
                                        AAIEnemyController* VictimController, ARougeDemoPlayerController* AttackController)
{
	//计算Kills
	ARougeDemoPlayerController* RougeDemoPlayerController = Cast<ARougeDemoPlayerController>(AttackController);
	if(RougeDemoPlayerController)
	{
		ARougeDemoPlayerState* AttackerPlayerState = RougeDemoPlayerController ? Cast<ARougeDemoPlayerState>(RougeDemoPlayerController->PlayerState) : nullptr;
		if(AttackerPlayerState)
		{
			AttackerPlayerState->AddToScore(1.f);
		}
	}
}

void ARougeDemoGameMode::RequetRespwan(ACharacter* ElimmedCharacter, AController* ElimmedController)
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

void ARougeDemoGameMode::Pause(bool bPause, bool bOverride)
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
