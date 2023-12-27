// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/EAbilityType.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "LevelMasterWidget.generated.h"

class UButton;
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
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* WS_LevelUp;

	UFUNCTION()
	void ResetUI();

	UPROPERTY(meta=(BindWidget))
	ULevelUpItemsComponentWidget* WBP_LevelUpItems;

	void AddSelection(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
		EPassiveAbilities PAbility,EAbilityType Type);

	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Intro;

private:
	void ResetLevelUpItems();

	UFUNCTION()
	void Btn_IntroClickedCallBack();

};
