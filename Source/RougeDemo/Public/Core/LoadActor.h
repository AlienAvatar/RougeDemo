// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "LoadActor.generated.h"

UCLASS()
class ROUGEDEMO_API ALoadActor : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Async Loading")
	void LoadLevel(FString Name);
	
private:
	FAsyncLoadGameFromSlotDelegate AsyncLoadingDelegate;
	
	UFUNCTION()
	void OnAsyncLoadComplete(const FString& LevelName, const int32 SlotIndex, USaveGame* SaveGame);

};
