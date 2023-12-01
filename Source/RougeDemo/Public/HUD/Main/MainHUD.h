// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainWidgetClass;

	void OnBegin();
private:
	UPROPERTY()
	UMainWidget* MainWidget;
};
