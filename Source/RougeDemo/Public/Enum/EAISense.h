#pragma once

UENUM(BlueprintType)
enum class EAISense : uint8
{
	EAI_None UMETA(DisplayName="None"),
	//视觉感知
	EAIS_Sight UMETA(DisplayName="Sight"),
	//听觉感知
	EAIS_Hearing UMETA(DisplayName="Hearing"),
	//伤害感知
	EAIS_Damage UMETA(DisplayName="Damage"),
};
