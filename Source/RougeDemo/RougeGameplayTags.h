#pragma once

#include "NativeGameplayTags.h"

struct FRougeGameplayTags
{
public:

	static const FRougeGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

public:
	//应用于所有的Ability Tag
	FGameplayTag Ability_ActivateFail_IsDead;
	FGameplayTag Ability_ActivateFail_Cooldown;
	FGameplayTag Ability_ActivateFail_Cost;
	FGameplayTag Ability_ActivateFail_TagsBlocked;
	FGameplayTag Ability_ActivateFail_TagsMissing;
	FGameplayTag Ability_ActivateFail_Networking;
	FGameplayTag Ability_ActivateFail_ActivationGroup;
	FGameplayTag Ability_Behavior_SurvivesDeath;
	
	FGameplayTag Ability_Type_Action_Lighting;
	FGameplayTag Ability_Type_Action_FireBall;
	FGameplayTag Ability_Type_Action_Hammer;
	FGameplayTag Ability_Type_Action_Test;

	//攻击分类
	FGameplayTag Ability_Type_Action_Melee_Light;
	FGameplayTag Ability_Type_Action_Melee_Heavy;

	FGameplayTag Ability_Type_Magic_Warrior_ActiveAbility;
	FGameplayTag Ability_Type_Magic_Warrior_PassiveAbility;
	FGameplayTag Ability_Type_Magic_Warrior_NormalAbility;
	FGameplayTag Ability_Type_Magic_Warrior_Max;
	
	FGameplayTag Ability_Type_Magic_Warrior_AA_Q_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Q_Level2;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Q_Level3;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Q_Level4;
	FGameplayTag Ability_Type_Magic_Warrior_AA_E_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_AA_E_Level2;
	FGameplayTag Ability_Type_Magic_Warrior_AA_E_Level3;
	FGameplayTag Ability_Type_Magic_Warrior_AA_E_Level4;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Shift_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Shift_Level2;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Shift_Level3;
	FGameplayTag Ability_Type_Magic_Warrior_AA_Shift_Level4;
	FGameplayTag Ability_Type_Magic_Warrior_AA_R_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_AA_R_Level2;
	FGameplayTag Ability_Type_Magic_Warrior_AA_R_Level3;
	FGameplayTag Ability_Type_Magic_Warrior_AA_R_Level4;
	
	FGameplayTag Ability_Type_Magic_Warrior_PA_X1_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_PA_X1_Level2;
	FGameplayTag Ability_Type_Magic_Warrior_PA_X2_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_PA_X2_Level2;

	FGameplayTag Ability_Type_Magic_Warrior_NA_X1_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_NA_X1_Level2;
	FGameplayTag Ability_Type_Magic_Warrior_NA_X2_Level1;
	FGameplayTag Ability_Type_Magic_Warrior_NA_X2_Level2;
	
	FGameplayTag Ability_Type_Attr_Heal;
	
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Look_Stick;
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_AutoRun;
	FGameplayTag InputTag_Sprint;
	FGameplayTag InputTag_Test;

	FGameplayTag InputTag_Ability_Q;
	FGameplayTag InputTag_Ability_E;
	FGameplayTag InputTag_Ability_Shift;
	FGameplayTag InputTag_Ability_R;
	// Initialization states for the GameFrameworkComponentManager, these are registered in order by LyraGameInstance and some actors will skip right to GameplayReady

	/** 功能已完成生成和初始复制，从 BeginPlay 调用*/
	FGameplayTag InitState_Spawned;

	/** 它会检查玩家状态和输入组件是否已就绪 */
	FGameplayTag InitState_DataAvailable;

	/** 所有数据都可用之后，该状态用于完成其他初始化操作，如添加Gameplay能力 */
	FGameplayTag InitState_DataInitialized;

	/** 对象已完成所有初始化，并准备好在正常Gameplay中进行交互 */
	FGameplayTag InitState_GameplayReady;

	FGameplayTag GameplayEvent_Death;
	FGameplayTag GameplayEvent_Reset;
	FGameplayTag GameplayEvent_RequestReset;

	FGameplayTag SetByCaller_Damage;
	FGameplayTag SetByCaller_Heal;

	FGameplayTag Cheat_GodMode;
	FGameplayTag Cheat_UnlimitedHealth;

	FGameplayTag Status_Crouching;
	FGameplayTag Status_AutoRunning;
	FGameplayTag Status_Death;
	FGameplayTag Status_Death_Dying;
	FGameplayTag Status_Death_Dead;

	FGameplayTag Movement_Mode_Walking;
	FGameplayTag Movement_Mode_NavWalking;
	FGameplayTag Movement_Mode_Falling;
	FGameplayTag Movement_Mode_Swimming;
	FGameplayTag Movement_Mode_Flying;
	FGameplayTag Movement_Mode_Custom;

	TMap<uint8, FGameplayTag> MovementModeTagMap;
	TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

protected:

	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode);
	void AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode);

private:

	static FRougeGameplayTags GameplayTags;
};



