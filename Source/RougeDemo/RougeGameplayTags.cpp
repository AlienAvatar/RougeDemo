#include "RougeGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

FRougeGameplayTags FRougeGameplayTags::GameplayTags;

void FRougeGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

void FRougeGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	AddTag(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability failed to activate because it is on cool down.");
	AddTag(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability failed to activate because it did not pass the cost checks.");
	AddTag(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	AddTag(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "Ability failed to activate because tags are missing.");
	AddTag(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");
	AddTag(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "Ability failed to activate because of its activation group.");
	
	AddTag(Ability_Type_Action_Lighting, "Ability.Type.Action.Lighting", "Lighting Ability");
	AddTag(Ability_Type_Action_FireBall, "Ability.Type.Action.FireBall", "Fire Ball Ability");
	AddTag(Ability_Type_Action_Test, "Ability.Type.Action.Testing", "Test");
	AddTag(Ability_Type_Action_Hammer, "Ability.Type.Action.Hammer", "Hammer Ability");
	AddTag(Ability_Type_Attr_Heal, "Ability.Type.Attr.Heal", "Health Heal");

	AddTag(Ability_Type_Magic_Warrior_ActiveAbility, "Ability.Type.Magic.Warrior.ActiveAbility", "Active Ability Group");
	AddTag(Ability_Type_Magic_Warrior_PassiveAbility, "Ability.Type.Magic.Warrior.PassiveAbility", "Passive Ability Group");
	AddTag(Ability_Type_Magic_Warrior_NormalAbility, "Ability.Type.Magic.Warrior.NormalAbility", "Normal Ability Group");
	AddTag(Ability_Type_Magic_Warrior_Max, "Ability.Type.Magic.Warrior.Max", "Current Ability Max");
	
	AddTag(Ability_Type_Magic_Warrior_AA_Q_Level1, "Ability.Type.Magic.Warrior.ActiveAbility.Q.Level1", "Active Ability1 Level1");
	AddTag(Ability_Type_Magic_Warrior_AA_Q_Level2, "Ability.Type.Magic.Warrior.ActiveAbility.Q.Level2", "Active Ability1 Level2");
	AddTag(Ability_Type_Magic_Warrior_AA_Q_Level3, "Ability.Type.Magic.Warrior.ActiveAbility.Q.Level3", "Active Ability1 Level3");
	AddTag(Ability_Type_Magic_Warrior_AA_Q_Level4, "Ability.Type.Magic.Warrior.ActiveAbility.Q.Level4", "Active Ability1 Level4");
	AddTag(Ability_Type_Magic_Warrior_AA_E_Level1, "Ability.Type.Magic.Warrior.ActiveAbility.E.Level1", "Active Ability1 Level1");
	AddTag(Ability_Type_Magic_Warrior_AA_E_Level2, "Ability.Type.Magic.Warrior.ActiveAbility.E.Level2", "Active Ability2 Level2");
	AddTag(Ability_Type_Magic_Warrior_AA_E_Level3, "Ability.Type.Magic.Warrior.ActiveAbility.E.Level3", "Active Ability2 Level3");
	AddTag(Ability_Type_Magic_Warrior_AA_E_Level4, "Ability.Type.Magic.Warrior.ActiveAbility.E.Level4", "Active Ability2 Level4");
	AddTag(Ability_Type_Magic_Warrior_AA_Shift_Level1, "Ability.Type.Magic.Warrior.ActiveAbility.Shift.Level1", "Active Ability1 Level1");
	AddTag(Ability_Type_Magic_Warrior_AA_Shift_Level2, "Ability.Type.Magic.Warrior.ActiveAbility.Shift.Level2", "Active Ability2 Level2");
	AddTag(Ability_Type_Magic_Warrior_AA_Shift_Level3, "Ability.Type.Magic.Warrior.ActiveAbility.Shift.Level3", "Active Ability2 Level3");
	AddTag(Ability_Type_Magic_Warrior_AA_Shift_Level4, "Ability.Type.Magic.Warrior.ActiveAbility.Shift.Level4", "Active Ability2 Level4");
	AddTag(Ability_Type_Magic_Warrior_AA_R_Level1, "Ability.Type.Magic.Warrior.ActiveAbility.R.Level1", "Active Ability1 Level1");
	AddTag(Ability_Type_Magic_Warrior_AA_R_Level2, "Ability.Type.Magic.Warrior.ActiveAbility.R.Level2", "Active Ability2 Level2");
	AddTag(Ability_Type_Magic_Warrior_AA_R_Level3, "Ability.Type.Magic.Warrior.ActiveAbility.R.Level3", "Active Ability2 Level3");

	
	AddTag(Ability_Type_Magic_Warrior_PA_X1_Level1, "Ability.Type.Magic.Warrior.PassiveAbility.X1.Level1", "Passive Ability1 Level1");
	AddTag(Ability_Type_Magic_Warrior_PA_X1_Level2, "Ability.Type.Magic.Warrior.PassiveAbility.X1.Level2", "Passive Ability1 Level2");
	AddTag(Ability_Type_Magic_Warrior_PA_X2_Level1, "Ability.Type.Magic.Warrior.PassiveAbility.X2.Level1", "Passive Ability2 Level1");
	AddTag(Ability_Type_Magic_Warrior_PA_X2_Level1, "Ability.Type.Magic.Warrior.PassiveAbility.X2.Level2", "Passive Ability2 Level2");

	AddTag(Ability_Type_Magic_Warrior_NA_X1_Level1, "Ability.Type.Magic.Warrior.NormalAbility.X1.Level1", "Normal Ability1 Level1");
	AddTag(Ability_Type_Magic_Warrior_NA_X1_Level2, "Ability.Type.Magic.Warrior.NormalAbility.X1.Level2", "Normal Ability1 Level2");
	AddTag(Ability_Type_Magic_Warrior_NA_X2_Level1, "Ability.Type.Magic.Warrior.NormalAbility.X2.Level1", "Normal Ability2 Level1");
	AddTag(Ability_Type_Magic_Warrior_NA_X2_Level1, "Ability.Type.Magic.Warrior.NormalAbility.X2.Level2", "Normal Ability2 Level2");

	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	AddTag(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	AddTag(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	AddTag(InputTag_AutoRun, "InputTag.AutoRun", "Auto-run input.");
	AddTag(InputTag_Sprint, "InputTag.Sprint", "Sprint input.");
	AddTag(InputTag_Test, "InputTag.Test", "Test input.");
	
	AddTag(InputTag_Ability_Q, "InputTag.Ability.Q", "Ability1 input.");
	AddTag(InputTag_Ability_E, "InputTag.Ability.E", "Ability2 input.");
	AddTag(InputTag_Ability_Shift, "InputTag.Ability.Shift", "Ability3 input.");
	AddTag(InputTag_Ability_R, "InputTag.Ability.R", "Ability4 input.");

	AddMovementModeTag(Movement_Mode_Walking, "Movement.Mode.Walking", MOVE_Walking);
	AddMovementModeTag(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", MOVE_NavWalking);
	AddMovementModeTag(Movement_Mode_Falling, "Movement.Mode.Falling", MOVE_Falling);
	AddMovementModeTag(Movement_Mode_Swimming, "Movement.Mode.Swimming", MOVE_Swimming);
	AddMovementModeTag(Movement_Mode_Flying, "Movement.Mode.Flying", MOVE_Flying);
	AddMovementModeTag(Movement_Mode_Custom, "Movement.Mode.Custom", MOVE_Custom);

	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");
	AddTag(Status_AutoRunning, "Status.AutoRunning", "Target is auto-running.");


	AddTag(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	
	AddTag(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	AddTag(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay effects.");

	AddTag(Status_Death, "Status.Death", "Target has the death status.");
	AddTag(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	AddTag(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");
}

void FRougeGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FRougeGameplayTags::AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode)
{
	AddTag(OutTag, TagName, "Character movement mode tag.");
	GameplayTags.MovementModeTagMap.Add(MovementMode, OutTag);
}

void FRougeGameplayTags::AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode)
{
	AddTag(OutTag, TagName, "Character custom movement mode tag.");
	GameplayTags.CustomMovementModeTagMap.Add(CustomMovementMode, OutTag);
}

FGameplayTag FRougeGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				//UE_LOG(LogLyra, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

