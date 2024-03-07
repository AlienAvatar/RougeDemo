// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Enum/EAbilityType.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "LevelMasterWidget.generated.h"

class ULevelUpCardComponentWidget;
class UButton;
class ULevelUpItemsComponentWidget;
class UWidgetSwitcher;
struct FGameplayTag;

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnCloseDelegate, FGameplayTag);
DECLARE_DELEGATE(FOnReadyOnDelegate);

UCLASS()
class ROUGEDEMO_API ULevelMasterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	ULevelUpItemsComponentWidget* WBP_LevelUpItems;

	void AddSelection(FText Name, FText Level, FText Desc, UTexture2D* Icon,  FText Type, FGameplayTag GameplayTag);

	void OnSelectedDelegateEventFunction(FGameplayTag GameplayTag);

	FOnCloseDelegate OnCloseDelegate;

	FOnReadyOnDelegate OnReadyDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Sub UI")
	TSubclassOf<ULevelUpCardComponentWidget> LevelUpCardComponentClass;

private:
	void ResetLevelUpItems();

	void Close();

	FGameplayTag Tmp_GameplayTag;
};
