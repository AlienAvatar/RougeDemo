// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EAbilityType.h"
#include "Enum/EActiveAbilities.h"
#include "Enum/EPassiveAbilities.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ControllerManagerInterface.h"
#include "RougePlayerController.generated.h"

class UDataTable;
class UAbilityComponent;
class ARougeHUD;
class ULevelMasterWidget;
/**
 * 
 */
UCLASS()
class ROUGEDEMO_API ARougePlayerController : public APlayerController, public IControllerManagerInterface
{
	GENERATED_BODY()

public:
	ARougePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AController interface
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~End of APlayerController interface
	
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDSkillValue(float SkillValue,float MaxSkillValue);

	virtual void OnLevelUp() override;

	virtual void UpdateLevelBar(float Percent, int32 Level) override;
	
	UPROPERTY()
	ULevelMasterWidget* LevelMasterWidget;

	void LevelUpMaxHealth(bool PowerUp);
	void LevelUpMaxSpeed(bool PowerUp);
	void LevelUpTimerReduction(bool PowerUp);
	void LevelUpAbilityDamage(bool PowerUp);

	//更新UI
	void UpdateHotbar();
protected:
	UPROPERTY()
	ARougeHUD* RougeDemoHUD;

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

	//创建主动技能的升级卡片
	void CreateActiveCard(int32 Local_MaxCount, TArray<EActiveAbilities>& Local_AvailableActiveAbilities, TMap<EActiveAbilities, int32>& Local_ActiveAbilitiesMap);

	//创建被动技能的升级卡片
	void CreatePassiveCard(int32 Local_MaxCount, TArray<EPassiveAbilities>& Local_AvailablePassiveAbilities, TMap<EPassiveAbilities, int32>& Local_PassiveAbilitiesMap);

	bool CanAddActiveAbility = true;
	bool CanAddPassiveAbility = true;

	void ProcessLevelUp(EAbilityType Type, EActiveAbilities ActiveAbilities, EPassiveAbilities PassiveAbilities);

	void AssignAbility(EAbilityType Type, EActiveAbilities ActiveAbilities, EPassiveAbilities PassiveAbilities);

	virtual void UpdateHudHotbar(TMap<EActiveAbilities, int32> ActiveMap, TMap<EPassiveAbilities, int32> PassiveMap) override;




};
