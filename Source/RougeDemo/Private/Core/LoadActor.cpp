// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LoadActor.h"

void ALoadActor::LoadLevel(FString Name)
{
	AsyncLoadingDelegate.BindUObject(this, &ALoadActor::OnAsyncLoadComplete);
	UGameplayStatics::AsyncLoadGameFromSlot(Name, 0, AsyncLoadingDelegate);
}

void ALoadActor::OnAsyncLoadComplete(const FString& LevelName, const int32 SlotIndex, USaveGame* SaveGame)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Load level %s complete"), *LevelName));
}
