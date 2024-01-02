// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/EAbilityType.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "LevelMasterWidget.generated.h"

class ULevelUpCardComponentWidget;
class UButton;
class ULevelUpItemsComponentWidget;
class UWidgetSwitcher;
/**
 * 
 */

DECLARE_DELEGATE_ThreeParams(FOnCloseDelegate, EAbilityType, EActiveAbilities, EPassiveAbilities);

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

	void OnSelectedDelegateEventFunction();

	FOnCloseDelegate OnCloseDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Sub UI")
	TSubclassOf<ULevelUpCardComponentWidget> LevelUpCardComponentClass;

private:
	void ResetLevelUpItems();

	UFUNCTION()
	void Btn_IntroClickedCallBack();

	void Close();

	EActiveAbilities Tmp_AAbility;
	
	EPassiveAbilities Tmp_PAbility;

	EAbilityType Tmp_Type;

	
};
