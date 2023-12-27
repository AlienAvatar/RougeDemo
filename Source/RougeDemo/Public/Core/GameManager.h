// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class ARougeDemoPlayerController;

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

	TArray<ARougeDemoPlayerController*> PlayerControllerArr;

	void GameSetUp();
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

	
};
