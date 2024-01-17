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
#include "Core/GameModes/RougeExperienceDefinition.h"
#include "Core/GameModes/RougeExperienceManagerComponent.h"
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

void ARougeGameMode::InitGameState()
{
	Super::InitGameState();

	//等待扩展组件加载完成
	URougeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<URougeExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnRougeExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
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

void ARougeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}


void ARougeGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;
	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(URougeExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	
}

void ARougeGameMode::OnExperienceLoaded(const URougeExperienceDefinition* CurrentExperience)
{
	//遍历所有Controller
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}
