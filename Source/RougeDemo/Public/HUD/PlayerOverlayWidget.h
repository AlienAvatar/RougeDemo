// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlayWidget.generated.h"

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
};
