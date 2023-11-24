// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RougeDemoInstance.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Core/RougeDemoSaveGame.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"
#include "SaveGame/GlobalOptionsSaveGame.h"
#include "ContentStreaming.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

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
	//给SlotName和UserIndex赋值
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

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &URougeDemoInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &URougeDemoInstance::EndLoadingScreen);
}

URougeDemoSaveGame* URougeDemoInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void URougeDemoInstance::FadeInAndShowLoadingScreen()
{
	FString MapName = "TestMap";
	BeginLoadingScreen(MapName);
}

void URougeDemoInstance::FadeOutAndHideLoadingScreen()
{
	EndLoadingScreen(GetWorld());
}

void URougeDemoInstance::BeginLoadingScreen(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreen;
	//如果加载完成 自动关闭
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
	//需要玩家主动中断
	LoadingScreen.bWaitForManualStop = true;
	LoadingScreen.bAllowEngineTick = true;
	LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Normal;

	if(LoadingScreenClass)
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenClass);
		if(LoadingScreenWidget)
		{
			TSharedPtr<SWidget> LoadScreen = LoadingScreenWidget->TakeWidget();
			LoadingScreen.WidgetLoadingScreen = LoadScreen;
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("LoadingScreenWidget is nullptr"));
		}

		//LoadingScreen.MoviePaths.Add("Loading_Screen_HD_with_Sound");
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		//GetMoviePlayer()->OnMoviePlaybackFinished();
	}
	
}

void URougeDemoInstance::EndLoadingScreen(UWorld* LoadedWorld)
{
	UE_LOG(LogTemp, Log, TEXT("EndLoadingScreen"));
	if (!IsRunningDedicatedServer())
	{
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->RemoveFromParent();
			LoadingScreenWidget->MarkAsGarbage();
		}
	}
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

void URougeDemoInstance::LoadGameLevel()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	FadeInAndShowLoadingScreen();
	
	//UGameplayStatics::OpenLevel(GetWorld(),FName("TestMap"));
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


