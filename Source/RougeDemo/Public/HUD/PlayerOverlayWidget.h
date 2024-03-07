// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/AbilityLevelUp.h"
#include "PlayerOverlayWidget.generated.h"

class UAbilityTitleWidget;
class UHorizontalBox;
class UVerticalBox;
class UTextBlock;
class UProgressBar;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreAmount;

	/*UPROPERTY(meta=(BindWidget))
	UProgressBar* SkillValueBar;*/

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

	void BuildHotbar(TMap<FGameplayTag, FAbilityLevelUp> AbilityMap);

	//更新经验条
	void UpdateXP(float Percent);

	
};
