// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoInstance.h"

#include "Core/RougeDemoSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "RougeDemo/RougeDemo.h"
#include "SaveGame/GlobalOptionsSaveGame.h"

DEFINE_LOG_CATEGORY(GameInstance);

URougeDemoInstance::URougeDemoInstance()
	: SaveSlot(TEXT("SaveGameSlot"))
	, SaveUserIndex(0)
{
	
}

void URougeDemoInstance::Init()
{
	Super::Init();
	
	FString SlotName;
	int32 UserIndex;
	GetSaveSlotInfo(SlotName,UserIndex);
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, UserIndex, LoadGameFromSlotDelegate);
	if(LoadGameFromSlotDelegate.IsBound())
	{
		GAME_INSTANCE_LOG(Log, TEXT("Loaded Save Game"));
	}else
	{
		WriteSaveGame();
		GAME_INSTANCE_LOG(Log, TEXT("New Save Game"));
	}
}

URougeDemoSaveGame* URougeDemoInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

FGlobalOptionsStruct URougeDemoInstance::GetGlobalOptions()
{
	if(UGameplayStatics::DoesSaveGameExist(GlobalOptionsName, 0))
	{
		UGlobalOptionsSaveGame* SaveGame = Cast<UGlobalOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot(GlobalOptionsName, 0));
		if(SaveGame)
		{
			return SaveGame->GlobalOptions;
		}
	}else
	{
		UGlobalOptionsSaveGame* SaveGame = Cast<UGlobalOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlobalOptionsSaveGame::StaticClass()));
		if(SaveGame)
		{
			UGameplayStatics::SaveGameToSlot(SaveGame, GlobalOptionsName, 0);
			return SaveGame->GlobalOptions;
		}
	}
	GAME_INSTANCE_LOG(Error, TEXT("GetGlobalOptions() has error, Not Save Game, Please try agian."));
	return FGlobalOptionsStruct();
}

void URougeDemoInstance::SetGlobalOptions(FGlobalOptionsStruct NewGlobalOptions)
{
	//更改文件中的属性
	UGlobalOptionsSaveGame* SaveGame = Cast<UGlobalOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlobalOptionsSaveGame::StaticClass()));
	if(SaveGame)
	{
		SaveGame->GlobalOptions = GlobalOptions;
		UGameplayStatics::SaveGameToSlot(SaveGame, GlobalOptionsName, 0);
	}
	
}

void URougeDemoInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

void URougeDemoInstance::GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const
{
	SlotName = SaveSlot;
	UserIndex = SaveUserIndex;
}

bool URougeDemoInstance::WriteSaveGame()
{
	//可以存取
	if (bSavingEnabled)
	{
		if (bCurrentlySaving)
		{
			// 队列中只有一个位置
			bPendingSaveRequested = true;
			return true;
		}

		// 正在异步存取
		bCurrentlySaving = true;

		// 异步调用
		UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &URougeDemoInstance::HandleAsyncSave));
		return true;
	}
	return false;
}

void URougeDemoInstance::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	bCurrentlySaving = false;

	if(bPendingSaveRequested)
	{
		bPendingSaveRequested = false;
		WriteSaveGame();
	}
}


