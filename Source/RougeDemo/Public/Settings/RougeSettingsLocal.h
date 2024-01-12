// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RougeSettingsLocal.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static URougeSettingsLocal* Get();

	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }

private:
	//自定义键盘设置映射
	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;
};

