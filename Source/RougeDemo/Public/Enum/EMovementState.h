#pragma once

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_None UMETA(DisplayName="None"),
	EMS_Grounded UMETA(DisplayName="Grounded"),
	EMS_InAir UMETA(DisplayName="InAir"),
	EMS_Mantiling UMETA(DisplayName="Mantiling"),
	EMS_RagDoll UMETA(DisplayName="RagDoll"),
};
