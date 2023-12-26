#pragma once
#include "Engine/DataTable.h"

#include "AbilityLevelUp.generated.h"

USTRUCT(BlueprintType)
struct FAbilityLevelUp : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LevelUpText;
};
