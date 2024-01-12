// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "Struct/EnemySpawnType.h"
#include "Struct/EnemySpawnWave.h"
#include "GameManager.generated.h"

class ARougePlayerController;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class ROUGEDEMO_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<ARougePlayerController*> PlayerControllerArr;

	void GameSetUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnvQuery* EQSFindSpawnLocation;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateCharactersXP(float Percent, int32 Level);

	void PrepareLevelUp();

	void StartTimer();

	void EndGame();
private:
	void UpdateTimer();

	int32 Time = 0;

	int32 Minutes = 0;

	FText GameTime;

	//单位 Minute
	int32 MaxGameTime = 10;

	FTimerHandle UpdateTimerTimerHandle;

	void UpdateTimerCallBack();

	// spawn enemy
	void SpawnUp();

	void PrepareWaveElites();

	void PrepareWaveEnemies();

	FTimerHandle PrepareWaveEnemiesTimer;

	void SpawnWave();

	float SpawnWaveCoolDown = 7.0f;

	//检查波索引是否有敌人分配-如果没有-使用以前保存的敌人
	void UpdateEnemyWave();

	float CurrentEnemySpawnIndex = 0;

	TArray<FEnemySpawnWave> EnemySpawnWaveArr;

	int32 MinEnemiesPerWave = 10;

	int32 MaxEnemiesPerWave = 20;

	int32 NumberOfEnemiesToSpawn = 0;

	int32 CurrentEnemyCount = 0;

	int32 MaxEnemiesCount = 45;

	void FindSpawnLocation();

	UFUNCTION()
	void SpawnEnemy(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	TArray<FEnemySpawnType> PreparedEnemiesArr;

	void ContinueSpawn();

	int32 WaveIndex;
};
