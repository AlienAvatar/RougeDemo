// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "RougeExperienceManager.generated.h"

/**
 * 用于管理多个PIE
 */
UCLASS()
class ROUGEDEMO_API URougeExperienceManager : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static bool RequestToDeactivatePlugin(const FString PluginURL);
};
