#pragma once
#include "Engine/DataTable.h"

#include "FinisherDamage.generated.h"

USTRUCT(BlueprintType)
struct FFinisherDamage : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<float> DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bAlwaysKillWithLastHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bDeathCancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DeathCancel_AttackerDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DeathCancel_TargetDelay;
};
