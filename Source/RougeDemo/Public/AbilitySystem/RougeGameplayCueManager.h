// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "RougeGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	URougeGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static URougeGameplayCueManager* Get();
	
	//~UGameplayCueManager interface
	virtual void OnCreated() override;
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
	virtual bool ShouldSyncLoadMissingGameplayCues() const override;
	virtual bool ShouldAsyncLoadMissingGameplayCues() const override;
	//~End of UGameplayCueManager interface

	//更新单一游戏玩法提示主要资产的捆绑包
	void RefreshGameplayCuePrimaryAsset();
};
