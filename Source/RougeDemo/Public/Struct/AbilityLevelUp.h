#pragma once
#include "AbilitySystem/RougeAbilitySet.h"
#include "Engine/DataTable.h"
#include "RougeDemo/RougeGameplayTags.h"

#include "AbilityLevelUp.generated.h"

struct FGameplayTag;
struct FGameplayTagContainer;
class URougeGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityLevelUp : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityDesc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRougeAbilitySet_GameplayAbility GameplayAbility;
};
