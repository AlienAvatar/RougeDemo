// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define ECC_PlayerBackStabbed ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Attack ECollisionChannel::ECC_GameTraceChannel2

//#define ECC_AIPawn ECollisionChannel::ECC_GameCollisionChannel1

DECLARE_LOG_CATEGORY_EXTERN(GameInstance, Log, All);
#define GAME_INSTANCE_LOG(Verbosity, Format, ...) \
UE_LOG(GameInstance, Verbosity, Format, ##__VA_ARGS__)