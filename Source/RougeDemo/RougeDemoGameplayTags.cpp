#include "RougeGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

//#define UE_DEFINE_GAMEPLAY_TAG_COMMENT(TagName, Tag, Comment) FNativeGameplayTag TagName(UE_PLUGIN_NAME, UE_MODULE_NAME, Tag, TEXT(Comment), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD); static_assert(UE::GameplayTags::Private::HasFileExtension(__FILE__), "UE_DEFINE_GAMEPLAY_TAG_COMMENT can only be used in .cpp files, if you're trying to share tags across modules, use UE_DECLARE_GAMEPLAY_TAG_EXTERN in the public header, and UE_DEFINE_GAMEPLAY_TAG_COMMENT in the private .cpp");
namespace RougeGameplayTags
{
	//代替UE_DEFINE_GAMEPLAY_TAG_COMMENT
	//UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	FNativeGameplayTag InputTag_Move("CommonUser", "CommonUser", "InputTag.Move", L"Move input.",
								 ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);

	FNativeGameplayTag InitState_Spawned("CommonUser", "CommonUser", "InitState.Spawned", L"1: Actor/component has initially spawned and can be extended",
								 ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);

	FNativeGameplayTag InitState_DataAvailable("CommonUser", "CommonUser", "InitState.DataAvailable", L"1: Actor/component has initially spawned and can be extended",
							 ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	
	FNativeGameplayTag InitState_DataInitialized("CommonUser", "CommonUser", "InitState.DataInitialized", L"1: Actor/component has initially spawned and can be extended",
						 ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

	// These are mapped to the movement modes inside GetMovementModeTagMap()
	FNativeGameplayTag Movement_Mode_Walking("CommonUser", "CommonUser", "Movement.Mode.Walking", L"Default Character movement tag",
								 ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	FNativeGameplayTag Movement_Mode_NavWalking("CommonUser", "CommonUser", "Movement.Mode.NavWalking", L"Default Character movement tag",
								ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	FNativeGameplayTag Movement_Mode_Falling("CommonUser", "CommonUser", "Movement.Mode.Falling", L"Default Character movement tag",
								ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	FNativeGameplayTag Movement_Mode_Swimming("CommonUser", "CommonUser", "Movement.Mode.Swimming", L"Default Character movement tag",
								ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	FNativeGameplayTag Movement_Mode_Flying("CommonUser", "CommonUser", "Movement.Mode.Flying", L"Default Character movement tag",
								ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	FNativeGameplayTag Movement_Mode_Custom("CommonUser", "CommonUser", "Movement.Mode.Custom", L"Default Character movement tag",
								ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
	// Unreal Movement Modes
	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{ MOVE_Walking, Movement_Mode_Walking },
		{ MOVE_NavWalking, Movement_Mode_NavWalking },
		{ MOVE_Falling, Movement_Mode_Falling },
		{ MOVE_Swimming, Movement_Mode_Swimming },
		{ MOVE_Flying, Movement_Mode_Flying },
		{ MOVE_Custom, Movement_Mode_Custom }
	};
}
