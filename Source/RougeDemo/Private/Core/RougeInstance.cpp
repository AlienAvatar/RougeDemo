// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Core\RougeInstance.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Core/RougeSaveGame.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Lib/RougeDemoFunctionLibary.h"
#include "RougeDemo/RougeDemo.h"
#include "SaveGame/GlobalOptionsSaveGame.h"
#include "ContentStreaming.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Components/GameFrameworkComponentManager.h"
#include "RougeDemo/RougeGameplayTags.h"

DEFINE_LOG_CATEGORY(LogGameInstance);

URougeInstance::URougeInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, SaveSlot(TEXT("SaveGameSlot"))
	, SaveUserIndex(0)
{
	
}

void URougeInstance::Init()
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

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		const FRougeGameplayTags& GameplayTags = FRougeGameplayTags::Get();

		//在GameInstance初始化期间通过调用 RegisterInitState 向子系统注册, 共享给游戏中的所有Actor
		//如果必需的数据可用，它将过渡到 DataAvailable ，但还无法过渡到 DataInitialized
		//以尝试遵循4状态初始化链
		//InitState_Spawned 功能已完成生成和初始复制，从 BeginPlay 调用
		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		//它会检查玩家状态和输入组件是否已就绪
		//InitState_DataAvailable 功能需要的所有数据都已复制或加载，包括其他Actor上也可能需要复制的依赖关系
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		//InitState_DataInitialized 所有数据都可用之后，该状态用于完成其他初始化操作，如添加Gameplay能力
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		//InitState_GameplayReady 对象已完成所有初始化，并准备好在正常Gameplay中进行交互
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void URougeInstance::Shutdown()
{
	Super::Shutdown();
}

URougeSaveGame* URougeInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void URougeInstance::FadeInAndShowLoadingScreen()
{
	FString MapName = "TestMap";
}

void URougeInstance::FadeOutAndHideLoadingScreen()
{
	
}

FGlobalOptionsStruct URougeInstance::GetGlobalOptions()
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

void URougeInstance::SetGlobalOptions(FGlobalOptionsStruct NewGlobalOptions)
{
	//更改文件中的属性
	UGlobalOptionsSaveGame* SaveGame = Cast<UGlobalOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlobalOptionsSaveGame::StaticClass()));
	if(SaveGame)
	{
		SaveGame->GlobalOptions = GlobalOptions;
		UGameplayStatics::SaveGameToSlot(SaveGame, GlobalOptionsName, 0);
	}
}

void URougeInstance::LoadGameLevel()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(),FName("TestMap"));
}

void URougeInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

void URougeInstance::GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const
{
	SlotName = SaveSlot;
	UserIndex = SaveUserIndex;
}

bool URougeInstance::WriteSaveGame()
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
		UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &URougeInstance::HandleAsyncSave));
		return true;
	}
	return false;
}

void URougeInstance::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	bCurrentlySaving = false;

	if(bPendingSaveRequested)
	{
		bPendingSaveRequested = false;
		WriteSaveGame();
	}
}


