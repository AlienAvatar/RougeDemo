// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RougePawnData.generated.h"

class URougeInputConfig;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URougePawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Input")
	TObjectPtr<URougeInputConfig> InputConfig;
};
