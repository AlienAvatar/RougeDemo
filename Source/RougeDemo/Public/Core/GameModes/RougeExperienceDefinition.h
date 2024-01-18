// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RougeExperienceDefinition.generated.h"

class URougeExperienceActionSet;
class URougePawnData;
class UGameFeatureAction;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class ROUGEDEMO_API URougeExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URougeExperienceDefinition();

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<URougeExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<const URougePawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

};
