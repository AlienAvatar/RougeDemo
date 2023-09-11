#pragma once
#include "MovementSettings.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ToughnessValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxToughnessValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FMovementSettings MovementSettings;
};
