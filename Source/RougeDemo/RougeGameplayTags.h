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
	FGameplayTag Ability_Type_Attr_Heal;
	
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Look_Stick;
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_AutoRun;
	FGameplayTag InputTag_Sprint;
	FGameplayTag InputTag_Test;
	
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



