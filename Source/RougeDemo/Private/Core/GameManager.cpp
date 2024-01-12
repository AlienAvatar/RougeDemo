// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameManager.h"

#include "AI/BaseAI.h"
#include "..\..\Public\Core\RougePlayerController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Interface/ControllerManagerInterface.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RougeDemo/RougeDemo.h"

DEFINE_LOG_CATEGORY(LogGameManager);
// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	GameSetUp();
	//SpawnUp();
}



// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::GameSetUp()
{
	StartTimer();
}

void AGameManager::UpdateCharactersXP(float Percent, int32 Level)
{
	ARougePlayerController* RougeDemoPlayerController = Cast<ARougePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(RougeDemoPlayerController == nullptr) { return; }
	if (IControllerManagerInterface* ControllerImpl = Cast<IControllerManagerInterface>(RougeDemoPlayerController))
	{
		ControllerImpl->UpdateLevelBar(Percent, Level);
	}
}

void AGameManager::PrepareLevelUp()
{
	ARougePlayerController* RougeDemoPlayerController = Cast<ARougePlayerController>((UGameplayStatics::GetPlayerController(GetWorld(), 0)));
	if(RougeDemoPlayerController == nullptr) { return; }
	RougeDemoPlayerController->OnLevelUp();

	//所有Controller
	/*for(const auto ControllerRef : PlayerControllerArr)
	{
		ControllerRef->OnLevelUp();
	}*/
}

void AGameManager::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		UpdateTimerTimerHandle,
		this,
		&AGameManager::UpdateTimerCallBack,
		1.0f,
		true
	);
}

void AGameManager::EndGame()
{
	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerTimerHandle);
}

void AGameManager::UpdateTimer()
{
	Time++;
	if(Time > 59)
	{
		Time = Time - 60;
		++Minutes;
	}else
	{
		FText S1 = Time < 10 ? FText::FromString(FString::FromInt(0)) : FText::FromString(TEXT(""));
		FText S2 = FText::FromString(FString::FromInt(Time));
		FText S = FText::Format(FText::FromString(TEXT("{0}{1}")), S1, S2);
		
		FText M = FText::FromString(FString::FromInt(Minutes));
		GameTime = FText::Format(FText::FromString(TEXT("{0}:{1}")), Minutes, S);

		
		if(Minutes > MaxGameTime)
		{
			EndGame();
		}
	}
}

void AGameManager::UpdateTimerCallBack()
{
	UpdateTimer();

	ARougePlayerController* RougeDemoPlayerController = Cast<ARougePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(RougeDemoPlayerController == nullptr) { return; }
	if (IControllerManagerInterface* ControllerImpl = Cast<IControllerManagerInterface>(RougeDemoPlayerController))
	{
		ControllerImpl->UpdateTime(GameTime);
	}
}

void AGameManager::SpawnUp()
{
	PrepareWaveEnemies();

	GetWorld()->GetTimerManager().SetTimer(
		PrepareWaveEnemiesTimer,
		this,
		&AGameManager::SpawnWave,
		SpawnWaveCoolDown,
		true,
		-1
	);
}

void AGameManager::PrepareWaveElites()
{
	
}

void AGameManager::PrepareWaveEnemies()
{
	//首先-为游戏的每一分钟创建足够的物品
	for(int i = 0; i < MaxGameTime; ++i)
	{
		FEnemySpawnWave EnemySpawnWave;
		EnemySpawnWaveArr.Add(EnemySpawnWave);
	}
	
	const FString Path = "/Script/Engine.DataTable'/Game/RougeDemo/SRC/Data/DataTables/DT_EnemySpawnType.DT_EnemySpawnType'";
	UDataTable* Table =  Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));
	
	if(Table)
	{
		TArray<FName> OutRowNames;
		UDataTableFunctionLibrary::GetDataTableRowNames(Table,OutRowNames);

		for(int j = 0; j < OutRowNames.Num(); ++j)
		{
			FEnemySpawnType* OutRow = Table->FindRow<FEnemySpawnType>(OutRowNames[j],"");
			if(OutRow)
			{
				EnemySpawnWaveArr[OutRow->AllowedWave].SpawnArr.Add(*OutRow);
			}
		}
	}
}

void AGameManager::SpawnWave()
{
	//Calculate how many enemies we want to spawn in interval
	UpdateEnemyWave();

	CurrentEnemySpawnIndex = 0;

	NumberOfEnemiesToSpawn = UKismetMathLibrary::RandomIntegerInRange(MinEnemiesPerWave, MaxEnemiesPerWave);

	if(CurrentEnemyCount < MaxEnemiesCount && CurrentEnemySpawnIndex < NumberOfEnemiesToSpawn)
	{
		FindSpawnLocation();
	}
}

void AGameManager::UpdateEnemyWave()
{
	TArray<FEnemySpawnType> SpawnsArr = EnemySpawnWaveArr[WaveIndex].SpawnArr;
	if(SpawnsArr.IsEmpty())
	{
		return;
	}else
	{
		PreparedEnemiesArr = SpawnsArr;
	}
}

void AGameManager::FindSpawnLocation()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(Pawn && EQSFindSpawnLocation)
	{
		UEnvQueryInstanceBlueprintWrapper* EQSInstance = UEnvQueryManager::RunEQSQuery(
			GetWorld(),
			EQSFindSpawnLocation,
			Pawn,
			EEnvQueryRunMode::AllMatching,
			nullptr
		);

		//wait to found location
		EQSInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGameManager::SpawnEnemy);
	}
}

void AGameManager::SpawnEnemy(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(!PreparedEnemiesArr.IsEmpty())
	{
		TArray<FVector> ResultLocations;
		if(QueryInstance->GetQueryResultsAsLocations(ResultLocations))
		{
			int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, ResultLocations.Num()-1);
			FVector SpawnLocation = ResultLocations[RandomIndex];
			SpawnLocation += FVector(0.f, 0.f, 25.f);
			
			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			TSubclassOf<ABaseAI> EnemyClass = PreparedEnemiesArr[0].Enemy;
			if(!EnemyClass)
			{
				return;
			}
			ABaseAI* Enemy = GetWorld()->SpawnActor<ABaseAI>(
				EnemyClass,
				SpawnLocation,
				FRotator::ZeroRotator,
				ActorSpawnParameters
			);
			
			if(Enemy)
			{
				//Enemy计数
				++CurrentEnemySpawnIndex;
				//继续生成
				ContinueSpawn();
			}
		}else
		{
			FindSpawnLocation();
			GAME_LOG(Error, TEXT("ERROR: Did Not Find Spawn Location"));
		}
	}else
	{
		GAME_LOG(Error, TEXT("ERROR: Enemy data table not setup correctly!"));
	}
}

void AGameManager::ContinueSpawn()
{
	if(CurrentEnemyCount < MaxEnemiesCount && CurrentEnemySpawnIndex < NumberOfEnemiesToSpawn)
	{
		FindSpawnLocation();
	}
}

