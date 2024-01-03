// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityTitleWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UAbilityTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TB_Level;

	UPROPERTY(meta=(BindWidget))
	UImage* Img_Icon;
	
	void SetUp(int32 Level, UTexture2D* Texture);
};
