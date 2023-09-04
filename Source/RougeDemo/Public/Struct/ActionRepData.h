#pragma once

#include "ActionRepData.generated.h"

USTRUCT(BlueprintType)
struct FActionRepData
{
	GENERATED_BODY()

public:
	//该Action是否处于运行状态
	UPROPERTY()
	bool bIsRunning;

	//该Action的发起者
	UPROPERTY()
	AActor* Instigator;
};
