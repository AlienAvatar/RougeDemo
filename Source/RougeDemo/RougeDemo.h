// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define ECC_PlayerBackStabbed ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Attack ECollisionChannel::ECC_GameTraceChannel2
#define ECC_TRACE_ENEMY ECC_GameTraceChannel3

//#define ECC_AIPawn ECollisionChannel::ECC_GameCollisionChannel1

DECLARE_LOG_CATEGORY_EXTERN(LogGameInstance, Log, All);
#define GAME_INSTANCE_LOG(Verbosity, Format, ...) \
UE_LOG(LogGameInstance, Verbosity, Format, ##__VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(LogRougeWidget, Log, All);
#define WIDGET_LOG(Verbosity, Format, ...) \
UE_LOG(LogRougeWidget, Verbosity, Format, ##__VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(LogRougeController, Log, All);
#define CONTROLLER_LOG(Verbosity, Format, ...) \
UE_LOG(LogRougeController, Verbosity, Format, ##__VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(LogGameManager, Log, All);
#define GAME_LOG(Verbosity, Format, ...) \
UE_LOG(LogGameManager, Verbosity, Format, ##__VA_ARGS__)

ROUGEDEMO_API DECLARE_LOG_CATEGORY_EXTERN(LogRouge, Log, All);

ROUGEDEMO_API FString GetClientServerContextString(UObject* ContextObject = nullptr);

