// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RougeDemoFunctionLibary.generated.h"

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeDemoFunctionLibary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static URougeDemoInstance* GetRougeDemoGameInstance(UObject* WorldContextObject);

	//开始加载画面
	UFUNCTION(Category = Loading)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	//关闭加载画面
	UFUNCTION(Category = Loading)
	static void StopLoadingScreen();

	UFUNCTION()
	static UTexture2D* FindActionIcon(EActiveAbilities AAbility);

	UFUNCTION()
	static UTexture2D* FindPassiveIcon(EPassiveAbilities PAbility);
};
