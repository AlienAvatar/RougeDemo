#pragma once

UENUM(BlueprintType)
enum class EPassiveAbilities : uint8
{
	EPA_AbilityDamage UMETA(DisplayName="Ability Bonus Damage"),
	EPA_Health UMETA(DisplayName="Health Bonus"),
	EPA_CooldownReduction UMETA(DisplayName="Ability Cooldown Reduction"),
	EPA_Speed UMETA(DisplayName="Speed Bonus"),
};
