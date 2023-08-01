#pragma once

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	ECT_Unarmed UMETA(DisplayName="Unarmed"), //无武器攻击
	ECT_Melee UMETA(DisplayName="Melee"),//近战攻击
	ECT_Range UMETA(DisplayName="Range"),//远程攻击
	ECT_Magic UMETA(DisplayName="Magic"),//魔法攻击
	
};
