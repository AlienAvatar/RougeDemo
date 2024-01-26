// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougeGameMode.h"

#include "AI/AIEnemyController.h"
#include "AI/BaseAI.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "..\..\Public\Core\RougePlayerState.h"
#include "Assets/RougeAssetManager.h"
#include "Character/RougeCharacter.h"
#include "Character/RougePawnExtensionComponent.h"
#include "Core/RougeGameSession.h"
#include "Core/RougeGameState.h"
#include "Core/RougeReplayPlayerController.h"
#include "Core/GameModes/RougeExperienceDefinition.h"
#include "Core/GameModes/RougeExperienceManagerComponent.h"
#include "Core/GameModes/RougeWorldSettings.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "HUD/RougeHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Assets/RougeGameData.h"
#include "Components/RougePlayerSpawningManagerComponent.h"
#include "Core/RougePlayerState.h"

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

	//等待Experience管理组件加载完成
	URougeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<URougeExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnRougeExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ARougeGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const URougePawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ARougeGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	//延迟启动新玩家，直到URougeExperienceManagerComponent加载完成
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* ARougeGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (URougePlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<URougePlayerSpawningManagerComponent>())
	{
		//选择出生点
		return PlayerSpawningComponent->ChoosePlayerStart(Player);
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

APawn* ARougeGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                  const FTransform& SpawnTransform)
{
	//配置参数
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		//生成Pawn
 		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (URougePawnExtensionComponent* PawnExtComp = URougePawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const URougePawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}
	return nullptr;
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

const URougePawnData* ARougeGameMode::GetPawnDataForController(const AController* InController) const
{
	// 查看Pawn Data是否已经在PlayerState中设置
	// 问题无法通过模板参数获取URougePawnData
	if (InController != nullptr)
	{
		if (const ARougePlayerState* RougePS = InController->GetPlayerState<ARougePlayerState>())
		{
			if (const URougePawnData* PawnData = RougePS->GetPawnData())
			{
				return PawnData;
			}
		}
	}

	// 如果没设置，回到当前默认的current experience
	check(GameState);
	URougeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<URougeExperienceManagerComponent>();
	check(ExperienceComponent);
	//检查Experience是否加载完成
 	if (ExperienceComponent->IsExperienceLoaded())
	{
		const URougeExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}
	}

	//如果没有PawnData，则返回空
	return nullptr;
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

	// if (!ExperienceId.IsValid() && World->IsPlayInEditor())
	// {
	// 	ExperienceId = GetDefault<URougeDeveloperSettings>()->ExperienceOverride;
	// 	ExperienceIdSource = TEXT("DeveloperSettings");
	// }
	
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	if (!ExperienceId.IsValid())
	{
		if (ARougeWorldSettings* TypedWorldSettings = Cast<ARougeWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	URougeAssetManager& AssetManager = URougeAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogTemp, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	// 默认设置
	if (!ExperienceId.IsValid())
	{
		//@TODO: Pull this from a config setting or something
		//这里对应AssetManager中Primary Asset Type, FName为蓝图的Name
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("RougeExperienceDefinition"), FName("BP_Default_RougeExperienceDefinition"));
		ExperienceIdSource = TEXT("Default");
	}

	//匹配对应的资源
	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
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

void ARougeGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
#if WITH_SERVER_CODE
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		URougeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<URougeExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->ServerSetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
#endif
}

bool ARougeGameMode::IsExperienceLoaded() const
{
	check(GameState)
	URougeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<URougeExperienceManagerComponent>();
	check(ExperienceComponent);
	return ExperienceComponent->IsExperienceLoaded();
}
