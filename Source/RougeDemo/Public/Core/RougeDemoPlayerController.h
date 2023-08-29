// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RougeDemoPlayerController.generated.h"

class ARougeDemoHUD;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeDemoPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	ARougeDemoHUD* RougeDemoHUD;
	
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDSkillValue(float SkillValue,float MaxSkillValue);
};
