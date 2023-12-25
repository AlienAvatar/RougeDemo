// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelMasterWidget.generated.h"

class ULevelUpItemsComponentWidget;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ULevelMasterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* WS_LevelUp;

	UFUNCTION()
	void ResetUI();

	UPROPERTY(meta=(BindWidget))
	ULevelUpItemsComponentWidget* WBP_LevelUpItems;
private:
	void ResetLevelUpItems();
};
