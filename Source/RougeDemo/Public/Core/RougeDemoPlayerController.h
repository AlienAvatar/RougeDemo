// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ControllerManagerInterface.h"
#include "RougeDemoPlayerController.generated.h"

class UDataTable;
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

	UPROPERTY(EditDefaultsOnly, Category="DataTable")
	UDataTable* DT_ActiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category="DataTable")
	UDataTable* DT_PassiveAbilities;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<ULevelMasterWidget> LevelMasterWidgetClass;
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDSkillValue(float SkillValue,float MaxSkillValue);

	virtual void OnLevelUp() override;

	virtual void UpdateLevelBar(float Percent, int32 Level) override;
	
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

	//检查给出的卡片中是否有可以升级的
	bool CheckIfEVOReady(EActiveAbilities& Ability);

	//检查主动技能，防止某个技能重复出现
	TArray<EActiveAbilities> CheckActiveAbilities(TMap<EActiveAbilities, int32> ActiveMap, int32 MaxLevel);
	
	TArray<EPassiveAbilities> CheckPassiveAbilities(TMap<EPassiveAbilities, int32> PassiveMap, int32 MaxLevel);

	//更新玩家XP和Level
	void UpdateCharacterUI(float Percent, int32 Level);

	void TestAction();

	virtual void UpdateTime(FText Time) override;
};
