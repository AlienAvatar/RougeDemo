// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/Main/ButtonComponentWidget.h"
#include "MainWidget.generated.h"

class UButton;
class UButtonComponentWidget;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UButtonComponentWidget* Btn_StartGame;

	UPROPERTY(meta=(BindWidget))
	UButtonComponentWidget* Btn_Options;

	UPROPERTY(meta=(BindWidget))
	UButtonComponentWidget* Btn_QuitGame;
	
	FTest Test;
};
