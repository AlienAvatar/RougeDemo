#pragma once

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	EAT_Active UMETA(DisplayName="Active"),
	EAT_Passive UMETA(DisplayName="Passive"),
	EAT_Gold UMETA(DisplayName="Gold"),
	EAT_Health UMETA(DisplayName="Health"),
	EAT_Evolution UMETA(DisplayName="Evolution"),
};
