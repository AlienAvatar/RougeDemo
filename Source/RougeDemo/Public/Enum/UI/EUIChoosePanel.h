#pragma once

UENUM(BlueprintType)
enum class EUIChoosePanel : uint8
{
	EUI_CP_Forward UMETA(DisplayName="Forward"),
	EUI_CP_Backward UMETA(DisplayName="Backward"),
	EUI_CP_None UMETA(DisplayName="None"),
};
