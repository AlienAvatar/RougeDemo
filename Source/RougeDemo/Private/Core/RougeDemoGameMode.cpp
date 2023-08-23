// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoGameMode.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ARougeDemoGameMode::PlayEliminated(ARougeDemoCharacter* ElimmedCharacter,
                                        ARougeDemoPlayerController* VictimController, ARougeDemoPlayerController* AttackController)
{
	
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
