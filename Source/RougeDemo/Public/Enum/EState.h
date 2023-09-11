#pragma once

UENUM(BlueprintType)
enum class EState : uint8
{
	//休闲状态
	ES_Passive UMETA(DisplayName="Passive"),
	//不可用状态
	ES_Disabled UMETA(DisplayName="Disabled"),

	ES_Attacking UMETA(DisplayName="Attacking"),

	ES_Falling UMETA(DisplayName="Falling"),
	
	ES_Dead UMETA(DisplayName="Dead"),
	
	ES_Stun UMETA(DisplayName="Stun"),
};
