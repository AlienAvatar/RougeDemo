// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RougeExperienceDefinition.generated.h"

class URougeExperienceActionSet;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class ROUGEDEMO_API URougeExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URougeExperienceDefinition();

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<URougeExperienceActionSet>> ActionSets;
};
