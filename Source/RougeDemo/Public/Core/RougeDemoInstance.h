// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/GlobalOptionsStruct.h"
#include "RougeDemoInstance.generated.h"

class URougeDemoSaveGame;
/**
 * 
 */

UCLASS()
class ROUGEDEMO_API URougeDemoInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URougeDemoInstance();
	virtual void Init() override;

	FString SaveSlot;

	//FPlatformMisc::GetPlatformUserForUserIndex(UserIndex);
	//不同的平台使用不同的UserIndex
	int32 SaveUserIndex;

	FGlobalOptionsStruct GetGlobalOptions();

	void SetGlobalOptions(FGlobalOptionsStruct NewGlobalOptions);

	void LoadGameLevel();
protected:
	void SetSavingEnabled(bool bEnabled);

	void GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const;

	FAsyncLoadGameFromSlotDelegate LoadGameFromSlotDelegate;

	FAsyncSaveGameToSlotDelegate SaveGameToSlotDelegate;
private:
	bool bSavingEnabled;

	UPROPERTY()
	URougeDemoSaveGame* CurrentSaveGame;

	UPROPERTY()
	bool bCurrentlySaving;
	
	bool WriteSaveGame();

	//当异步存取的时候调用
	virtual void HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	//如果其他线程正在存取中，则返回true
	bool bPendingSaveRequested;

	URougeDemoSaveGame* GetCurrentSaveGame();

	FString GlobalOptionsName = "GlobalOptions";

	FGlobalOptionsStruct GlobalOptions;

	void FadeInAndShowLoadingScreen();

	void PlayLoadingScreen();
};
