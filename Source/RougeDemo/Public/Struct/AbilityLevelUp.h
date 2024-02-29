#pragma once
#include "Engine/DataTable.h"
#include "RougeDemo/RougeGameplayTags.h"

#include "AbilityLevelUp.generated.h"

struct FGameplayTag;
struct FGameplayTagContainer;

USTRUCT(BlueprintType)
struct FAbilityLevelUp : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LevelUpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;
};
