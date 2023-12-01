#pragma once

UENUM(BlueprintType)
enum class EUIState : uint8
{
	//Quit
	EUIS_Quit UMETA(DisplayName="Quit"),
	//Start
	EUIS_Start UMETA(DisplayName="Start"),
	//Options
	EUIS_Options UMETA(DisplayName="Options"),
};
