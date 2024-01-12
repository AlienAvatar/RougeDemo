#pragma once

#include "NativeGameplayTags.h"

namespace RougeGameplayTags
{
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);

	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	ROUGEDEMO_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	//这些是从MovementMode枚举到GameplayTags与这些枚举相关联的映射(如下)
	ROUGEDEMO_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
}
