#pragma once

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	EMD_Forward UMETA(DisplayName="Forward"),
	EMD_Backward UMETA(DisplayName="Backward"),
	EMD_Right UMETA(DisplayName="Right"),
	EMD_Left UMETA(DisplayName="Left"),

	EMD_ForwardRight UMETA(DisplayName="ForwardRight"),
	EMD_ForwardLeft UMETA(DisplayName="ForwardLeft"),
	EMD_BackwardRight UMETA(DisplayName="BackwardRight"),
	EMD_BackwardLeft UMETA(DisplayName="BackwardLeft"),

	EMD_MAX UMETA(DisplayName="None"),
};
