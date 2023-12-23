#pragma once

UENUM(BlueprintType)
enum class EUIMainMenu : uint8
{
	EUI_MM_Start UMETA(DisplayName="Start"),
	EUI_MM_Options UMETA(DisplayName="Options"),
	EUI_MM_Quit UMETA(DisplayName="Quit"),
	EUI_MM_None UMETA(DisplayName="None"),
};
