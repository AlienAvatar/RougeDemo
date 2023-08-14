#pragma once
#include "Engine/DataTable.h"

#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	
};
