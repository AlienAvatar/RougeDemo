#pragma once

UENUM(BlueprintType)
enum class EFinisherMoveToSettings : uint8
{
	EFMTS_AttackerToTarget UMETA(DisplayName="AttackerToTarget"),
	EFMTS_TargetToAttack UMETA(DisplayName="TargetToAttacker"),
	
};
