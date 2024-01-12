// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RougeHUD.generated.h"

class UPlayerOverlayWidget;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,Category="Player State")
	TSubclassOf<UUserWidget> PlayerOverlayClass;
	
	UPROPERTY()
	UPlayerOverlayWidget* PlayerOverlayWidget;

	
protected:
	virtual void BeginPlay() override;

	void AddPlayerOverlay();


};
