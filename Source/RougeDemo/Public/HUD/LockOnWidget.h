// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOnWidget.generated.h"

class UOverlay;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ULockOnWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UOverlay* OV_LOCK;

	UPROPERTY()
	AActor* LockOnTargetRef;
public:
	void ToggleMarket(AActor* LockOnTarget);
};
