// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/GlobalOptionsStruct.h"
#include "RougeInstance.generated.h"

class URougeSaveGame;
/**
 * 
 */

UCLASS()
class ROUGEDEMO_API URougeInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URougeInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	FString SaveSlot;

	//FPlatformMisc::GetPlatformUserForUserIndex(UserIndex);
	//不同的平台使用不同的UserIndex
	int32 SaveUserIndex;

	FGlobalOptionsStruct GetGlobalOptions();

	void SetGlobalOptions(FGlobalOptionsStruct NewGlobalOptions);

	void LoadGameLevel();
protected:
	// ~ UGameInstance Interface Begin
	virtual void Init() override;
	virtual void Shutdown() override;
	// ~ UGameInstance Interface End
	
	void SetSavingEnabled(bool bEnabled);

	void GetSaveSlotInfo(FString& SlotName, int32& UserIndex) const;

	FAsyncLoadGameFromSlotDelegate LoadGameFromSlotDelegate;

	FAsyncSaveGameToSlotDelegate SaveGameToSlotDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading Screen")
	TSubclassOf<UUserWidget> LoadingScreenClass;

	UPROPERTY()
	UUserWidget* LoadingScreenWidget;

	FTimerHandle LoadingScreenTimer;
private:
	bool bSavingEnabled;

	UPROPERTY()
	URougeSaveGame* CurrentSaveGame;

	UPROPERTY()
	bool bCurrentlySaving;
	
	bool WriteSaveGame();

	//当异步存取的时候调用
	virtual void HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	//如果其他线程正在存取中，则返回true
	bool bPendingSaveRequested;

	URougeSaveGame* GetCurrentSaveGame();

	FString GlobalOptionsName = "GlobalOptions";

	FGlobalOptionsStruct GlobalOptions;

	void FadeInAndShowLoadingScreen();

	void FadeOutAndHideLoadingScreen();
};
