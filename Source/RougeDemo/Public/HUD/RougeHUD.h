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

	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface
	
	/*
	UPROPERTY(EditAnywhere,Category="Player State")
	TSubclassOf<UUserWidget> PlayerOverlayClass;
	*/
	
	UPROPERTY()
	UPlayerOverlayWidget* PlayerOverlayWidget;

	void AddPlayerOverlay(TSubclassOf<UUserWidget> PlayerOverlayClass);
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	



};
