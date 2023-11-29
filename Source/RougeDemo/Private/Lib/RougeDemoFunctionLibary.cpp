// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/RougeDemoFunctionLibary.h"

#include "Core/RougeDemoInstance.h"
#include "Kismet/GameplayStatics.h"

URougeDemoInstance* URougeDemoFunctionLibary::GetRougeDemoGameInstance(UObject* WorldContextObject)
{
	return Cast<URougeDemoInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

void URougeDemoFunctionLibary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	/*IRougeDemoLoadingScreenModule& LoadingScreenModule = IRougeDemoLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);*/
}

void URougeDemoFunctionLibary::StopLoadingScreen()
{
	/*IRougeDemoLoadingScreenModule& LoadingScreenModule = IRougeDemoLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();*/
}
