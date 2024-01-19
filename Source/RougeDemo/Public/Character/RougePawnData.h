// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RougePawnData.generated.h"

class URougeInputConfig;
class URougeAbilitySet;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URougePawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Input")
	TObjectPtr<URougeInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Pawn")
	TSubclassOf<APawn> PawnClass;

	//赋予Pawn Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Abilities")
	TArray<TObjectPtr<URougeAbilitySet>> AbilitySets;
protected:


};
