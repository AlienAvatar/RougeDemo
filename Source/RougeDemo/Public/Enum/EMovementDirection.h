#pragma once

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	EMD_Forward UMETA(DisplayName="Forward"),
	EMD_Backward UMETA(DisplayName="Backward"),
	EMD_Right UMETA(DisplayName="Right"),
	EMD_Left UMETA(DisplayName="Left")
};
