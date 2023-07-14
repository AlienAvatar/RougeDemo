#pragma once

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	ERM_VelocityDirection UMETA(DisplayName="VelocityDirection"),
	ERM_LookingDirection UMETA(DisplayName="LookingDirection"),
	ERM_Aiming UMETA(DisplayName="Aiming")
};
