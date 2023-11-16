// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
//#include "Styling/SlateBrush.h"
#include "ItemBase.generated.h"


/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class ROUGEDEMO_API UItemBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UItemBase()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
		, AbilityLevel(1)
	{}

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	/** Icon to display */
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FSlateBrush ItemIcon;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxCount;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
	bool IsConsumable() const;

	/** Maximum level this item can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxLevel;

	/** Ability to grant if this item is slotted */
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<URPGGameplayAbility> GrantedAbility;*/

	/** Ability level this item grants. <= 0 means the character level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 AbilityLevel;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
