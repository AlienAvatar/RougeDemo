// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "PlayerOverlayWidget.generated.h"

class UAbilityTitleWidget;
class UHorizontalBox;
class UVerticalBox;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreAmount;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* SkillValueBar;

	UPROPERTY()
	UVerticalBox* VB_Skill;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* PB_XP;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TB_Level;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TB_Time;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* HB_Active;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* HB_Passive;

	UPROPERTY(EditAnywhere, Category= "SubUI")
	TSubclassOf<UUserWidget> AbilityTitleWidgetClass;

	void UpdateTime(FText Time);

	void BuildHotbar(TMap<EActiveAbilities, int32> ActiveMap, TMap<EPassiveAbilities, int32> PassiveMap);
};
