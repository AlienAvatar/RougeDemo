// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RougeGameData.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URougeGameData();

	//返回GameData
	static const URougeGameData& Get();

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;

	// Gameplay effect used to apply damage.  Uses SetByCaller for the damage magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Damage Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffect_SetByCaller;

	// Gameplay effect used to apply healing.  Uses SetByCaller for the healing magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Heal Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> HealGameplayEffect_SetByCaller;
};
