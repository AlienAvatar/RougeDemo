// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UChooseWidget;
class UMainWidget;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	UMainWidget* GetMainWidget() const { return MainWidget; }
	void ShowChooseWidget();
	void HideMainWidget();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ChooseWidgetClass;
	
	void OnBegin();
private:
	UPROPERTY()
	UMainWidget* MainWidget;

	UPROPERTY()
	UChooseWidget* ChooseWidget;
};
