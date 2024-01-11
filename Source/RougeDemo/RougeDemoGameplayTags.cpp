#include "RougeDemoGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

//#define UE_DEFINE_GAMEPLAY_TAG_COMMENT(TagName, Tag, Comment) FNativeGameplayTag TagName(UE_PLUGIN_NAME, UE_MODULE_NAME, Tag, TEXT(Comment), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD); static_assert(UE::GameplayTags::Private::HasFileExtension(__FILE__), "UE_DEFINE_GAMEPLAY_TAG_COMMENT can only be used in .cpp files, if you're trying to share tags across modules, use UE_DECLARE_GAMEPLAY_TAG_EXTERN in the public header, and UE_DEFINE_GAMEPLAY_TAG_COMMENT in the private .cpp");
namespace RougeDemoGameplayTags
{
	//代替UE_DEFINE_GAMEPLAY_TAG_COMMENT
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
}
