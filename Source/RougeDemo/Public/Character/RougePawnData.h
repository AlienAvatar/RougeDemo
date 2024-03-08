// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RougePawnData.generated.h"

class URougeInputConfig;
class URougeAbilitySet;
class URougeAbilityTagRelationshipMapping;
class ULevelMasterWidget;
class UDataTable;
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

	// 用于这个Pawn的Action映射的ability tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rouge|Abilities")
	TObjectPtr<URougeAbilityTagRelationshipMapping> TagRelationshipMapping;

	UPROPERTY(EditAnywhere, Category="PlayerHUD")
	TSubclassOf<UUserWidget> PlayerOverlayClass;

	UPROPERTY(EditAnywhere, Category="PlayerHUD")
	TSubclassOf<ULevelMasterWidget> LevelMasterWidgetClass;

	UPROPERTY(EditAnywhere, Category="Data")
	UDataTable* DT_ActiveAbilities;

	UPROPERTY(EditAnywhere, Category="Data")
	UDataTable* DT_PassiveAbilities;

	UPROPERTY(EditAnywhere, Category="Data")
	UDataTable* DT_UltimateAbilities;

	UPROPERTY(EditAnywhere, Category="Data")
	UDataTable* DT_NormalAbilities;
protected:


};
