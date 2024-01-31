// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RougeAbilityTagRelationshipMapping.generated.h"

/** Struct that defines the relationship between different ability tags */
USTRUCT()
struct FLyraAbilityTagRelationship
{
	GENERATED_BODY()

	/** 这个容器关系所涉及的Tag，一个Ability能有多个Tag */
	UPROPERTY(EditAnywhere, Category = Ability, meta = (Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	/** 其他能力Tag将被任何使用这个Tag的能力所阻挡 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	/**  其他能力Tag将被任何使用这个Tag的能力所取消 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	/** 如果一个Ability有这个Tag，隐式的添加到Ability的激活所需Tag中*/
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	/** 如果一个Ability有这个Tag，隐式的添加到Ability的阻挡所需Tag中*/
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationBlockedTags;
};

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API URougeAbilityTagRelationshipMapping : public UDataAsset
{
	GENERATED_BODY()

private:
	/** The list of relationships between different gameplay tags (which ones block or cancel others) */
	UPROPERTY(EditAnywhere, Category = Ability, meta=(TitleProperty="AbilityTag"))
	TArray<FLyraAbilityTagRelationship> AbilityTagRelationships;
};
