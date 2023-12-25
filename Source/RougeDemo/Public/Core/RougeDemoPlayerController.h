// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ControllerManagerInterface.h"
#include "RougeDemoPlayerController.generated.h"

class UAbilityComponent;
class ARougeDemoHUD;
class ULevelMasterWidget;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougeDemoPlayerController : public APlayerController, public IControllerManagerInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	ARougeDemoHUD* RougeDemoHUD;

	// GAS will instead it
	UPROPERTY()
	UAbilityComponent* AbilityComponent; 
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDSkillValue(float SkillValue,float MaxSkillValue);

	virtual void OnLevelUp() override;

	UPROPERTY()
	ULevelMasterWidget* LevelMasterWidget;
private:
	void SetupPlayer();

	void SetReference();

	//当每次升级后，弹出升级界面
	void CreateLevelUpUI();
	
	bool bPreparingUI = false;
	bool bLevelUpHudUp = false;

	//创建界面
	void PrepareLevelUp();

	//设置数据
	void ExecuteLevelUp();
};
